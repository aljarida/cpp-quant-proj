import yfinance as yf
import pandas as pd
import os

from printer import Printer
from enums import DownloadStatus, SAVE_DIR, NYSE_PATH, US_NASDAQ_PATH

DEBUG: bool = False



def tickers_with_csvs(directory: str) -> set[str]:
    return {
        os.path.splitext(filename)[0]
        for filename in os.listdir(directory)
        if filename.endswith('.csv') and os.path.isfile(os.path.join(directory, filename))
    }

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
    Printer.print_download_stats(stats)

def sanitize_ticker(ticker: str) -> str:
    if '^' in ticker:
        return ticker.replace('^', '-P')
    
    if '/' in ticker:
        if ticker.endswith('/U'):
            return ticker.replace('/U', '-UN')
        elif ticker.endswith('/W'):
            return ticker.replace('/W', '-WT')
        elif ticker.endswith('/R'):
            return ticker.replace('/R', '-RT')
        else:
            return ticker.replace('/', '-')

    return ticker

def extract_tickers(*file_paths: str) -> list[str]:
    res: set[str] = set()

    for path in file_paths:
        if not os.path.exists(path):
            raise FileNotFoundError(f"File not found: {path}")
        
        df: pd.DataFrame = pd.read_csv(path)

        if "Symbol" not in df.columns:
            raise ValueError(f"'Symbol' column not found in {path}. Columns: {list(df.columns)}")
    
        res = res.union(set(df["Symbol"].dropna().astype(str).apply(sanitize_ticker).tolist()))
    
    return list(res)



if __name__ == "__main__":
    print(f"Downloading data for tickers...")
    all_tickers: list[str] = extract_tickers(NYSE_PATH, US_NASDAQ_PATH)
    if DEBUG: # Only download a subset of all the tickers.
        tickers_to_grab = all_tickers[:10]
    else:
        already_downloaded: set[str] = tickers_with_csvs(SAVE_DIR)
        tickers_to_grab = [t for t in all_tickers if t not in already_downloaded] 
    
    download_all_tickers(tickers_to_grab)
