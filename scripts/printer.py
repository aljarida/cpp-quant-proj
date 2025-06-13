import sys

from enums import DownloadStatus

class Printer():
    @staticmethod
    def above_progress(message: str) -> None:
        sys.stdout.write("\033[1A")        # Move cursor up
        sys.stdout.write("\033[2K")        # Clear line
        sys.stdout.write(message + "\n")   # Print message
        sys.stdout.flush()

    @staticmethod
    def progress_bar(current: int, total: int, width: int = 50) -> None:
        progress = int(width * current / total)
        bar: str = f"[{'=' * progress}{'.' * (width - progress)}] {current}/{total} {int(current * 100 / total)}%"
        sys.stdout.write("\0337")              # Save cursor position
        sys.stdout.write("\033[s")             # Save again (redundancy for some terminals)
        sys.stdout.write("\033[999B")          # Move cursor way down
        sys.stdout.write("\r" + bar)           # Carriage return + print progress
        sys.stdout.write("\0338")              # Restore cursor position
        sys.stdout.flush()

    @staticmethod
    def return_carriage() -> None:
        sys.stdout.write("\033[999B")   # Move cursor down
        sys.stdout.write("\n")          # Print newline so future output is below
        sys.stdout.flush()
   
    @staticmethod 
    def print_download_stats(stats: dict[DownloadStatus, list[str]]) -> None:
        len_success: int = len(stats[DownloadStatus.DOWNLOAD_SUCCESS])
        len_existing: int = len(stats[DownloadStatus.ALREADY_EXISTS])
        len_no_data: int = len(stats[DownloadStatus.NO_DATA_FOUND])
        len_bad_request: int = len(stats[DownloadStatus.REQUEST_ERROR])

        print(f"Successfully downloaded {len_success}.")
        print(f"Skipped downloading {len_existing} already-downloaded files.")
        print(f"Failed to find data for {len_no_data} stocks.")
        print(f"Failed to request data for {len_bad_request} stocks.")
