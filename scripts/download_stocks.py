import yfinance as yf
import pandas as pd
import os
import sys

from enum import Enum, auto

DEBUG: bool = False

SAVE_DIR: str = "../data/stocks"
NYSE_PATH: str = "../data/ticker_symbols/nyse.csv"
US_NASDAQ_PATH: str = "../data/ticker_symbols/us_nasdaq.csv"

class Printer():
    @staticmethod
    def above_progress(message: str):
        sys.stdout.write("\033[1A")        # Move cursor up
        sys.stdout.write("\033[2K")        # Clear line
        sys.stdout.write(message + "\n")   # Print message
        sys.stdout.flush()

    @staticmethod
    def progress_bar(current: int, total: int, width=50):
        progress = int(width * current / total)
        bar: str = f"[{'=' * progress}{'.' * (width - progress)}] {current}/{total} {int(current * 100 / total)}%"
        sys.stdout.write("\0337")              # Save cursor position
        sys.stdout.write("\033[s")             # Save again (redundancy for some terminals)
        sys.stdout.write("\033[999B")          # Move cursor way down
        sys.stdout.write("\r" + bar)           # Carriage return + print progress
        sys.stdout.write("\0338")              # Restore cursor position
        sys.stdout.flush()

    @staticmethod
    def return_carriage():
        sys.stdout.write("\033[999B")   # Move cursor down
        sys.stdout.write("\n")          # Print newline so future output is below
        sys.stdout.flush()

class DownloadStatus(Enum):
    ALREADY_EXISTS = auto()
    NO_DATA_FOUND = auto()
    DOWNLOAD_SUCCESS = auto()
    REQUEST_ERROR = auto()

def fetch_daily_stock_data(ticker_symbol: str, save_dir: str) -> DownloadStatus:
    os.makedirs(save_dir, exist_ok=True)
    filepath = os.path.join(save_dir, f"{ticker_symbol}.csv")

    if os.path.exists(filepath):
        Printer.above_progress(f"{ticker_symbol} already exists at {filepath}; skipping.")
        return DownloadStatus.ALREADY_EXISTS

    Printer.above_progress(f"Downloading data for {ticker_symbol}...")
    ticker: yf.Ticker = yf.Ticker(ticker_symbol)
    df = ticker.history(period="max", interval="1d", auto_adjust=False)

    if df.empty:
        Printer.above_progress(f"No data found for {ticker_symbol}.")
        return DownloadStatus.NO_DATA_FOUND

    df.reset_index(inplace=True)
    df.columns = [col.lower().replace(' ', '_') for col in df.columns]

    required_columns = ['date', 'open', 'high', 'low', 'close', 'volume']
    df = df[required_columns]
    if len(required_columns) > len(df.columns):
        return DownloadStatus.NO_DATA_FOUND

    df.to_csv(filepath, index=False)
    Printer.above_progress(f"Saved {ticker_symbol} to {filepath}.")

    return DownloadStatus.DOWNLOAD_SUCCESS

def print_download_stats(stats: dict[DownloadStatus, list[str]]) -> None:
    len_success: int = len(stats[DownloadStatus.DOWNLOAD_SUCCESS])
    len_existing: int = len(stats[DownloadStatus.ALREADY_EXISTS])
    len_no_data: int = len(stats[DownloadStatus.NO_DATA_FOUND])
    len_bad_request: int = len(stats[DownloadStatus.REQUEST_ERROR])

    print(f"Successfully downloaded {len_success}.")
    print(f"Skipped downloading {len_existing} already-downloaded files.")
    print(f"Failed to find data for {len_no_data} stocks" + (":" if len_no_data else "."))
    _ = [print(ticker) for ticker in stats[DownloadStatus.NO_DATA_FOUND]]
    print(f"Failed to request data for {len_bad_request} stocks" + (":" if len_bad_request else "."))
    _ = [print(ticker) for ticker in stats[DownloadStatus.REQUEST_ERROR]]

def download_all_tickers(ticker_list: list[str], save_dir: str = SAVE_DIR) -> None:
    stats: dict[DownloadStatus, list[str]] = {
        DownloadStatus.ALREADY_EXISTS: [],
        DownloadStatus.NO_DATA_FOUND: [],
        DownloadStatus.DOWNLOAD_SUCCESS: [],
        DownloadStatus.REQUEST_ERROR: [],
    }

    for i, ticker in enumerate(ticker_list, start=1):
        try:
            status: DownloadStatus = fetch_daily_stock_data(ticker, save_dir)
            stats[status].append(ticker)
        except Exception as e:
            stats[DownloadStatus.REQUEST_ERROR].append(ticker)
            Printer.above_progress(f"Error fetching {ticker}: {e}!")
        
        Printer.progress_bar(i, len(ticker_list))
    
    Printer.return_carriage()
    print_download_stats(stats)

def extract_tickers(*file_paths: str) -> list[str]:
    res: set[str] = set()

    for path in file_paths:
        if not os.path.exists(path):
            raise FileNotFoundError(f"File not found: {path}")
        
        df: pd.DataFrame = pd.read_csv(path)

        if "Symbol" not in df.columns:
            raise ValueError(f"'Symbol' column not found in {path}. Columns: {list(df.columns)}")
    
        res = res.union(set(df["Symbol"].dropna().astype(str).tolist()))
    
    return list(res)

if __name__ == "__main__":
    tickers: list[str] = extract_tickers(NYSE_PATH, US_NASDAQ_PATH)
    if DEBUG: # Only download a subset of all the tickers.
        tickers = tickers[:5] + tickers[-5:]
        print(f"Downloading data for {tickers}...")
    
    download_all_tickers(tickers)
