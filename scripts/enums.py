from enum import Enum, auto

SAVE_DIR: str = "../data/stocks"
NYSE_PATH: str = "../data/ticker_symbols/nyse.csv"
US_NASDAQ_PATH: str = "../data/ticker_symbols/us_nasdaq.csv"

class DownloadStatus(Enum):
    ALREADY_EXISTS = auto()
    NO_DATA_FOUND = auto()
    DOWNLOAD_SUCCESS = auto()
    REQUEST_ERROR = auto()
