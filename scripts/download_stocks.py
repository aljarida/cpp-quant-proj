import yfinance as yf
import pandas as pd
import os

def fetch_daily_stock_data(ticker_symbol: str, save_dir: str = "data") -> pd.DataFrame:
    """
    Fetches and saves daily historical stock data if not already saved.

    Args:
        ticker_symbol (str): Stock ticker like 'MSFT'.
        save_dir (str): Directory to save CSVs.

    Returns:
        pd.DataFrame: The stock's historical data.
    """
    os.makedirs(save_dir, exist_ok=True)
    filepath = os.path.join(save_dir, f"{ticker_symbol}.csv")

    if os.path.exists(filepath):
        print(f"✔️ {ticker_symbol} already exists at {filepath}, skipping.")
        return pd.read_csv(filepath, parse_dates=["Date"])

    print(f"📥 Downloading data for {ticker_symbol}...")

    ticker = yf.Ticker(ticker_symbol)
    df = ticker.history(period="max", interval="1d", auto_adjust=False)

    if df.empty:
        print(f"⚠️ No data found for {ticker_symbol}")
        return pd.DataFrame()

    df.reset_index(inplace=True)
    df.columns = [col.lower().replace(' ', '_') for col in df.columns]
    df.to_csv(filepath, index=False)
    print(f"💾 Saved {ticker_symbol} to {filepath}")

    return df

