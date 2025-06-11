#include "market_data.hpp"

#include <iostream>

int main()
{
    MarketDataFeed feed("../data/IBM.csv");

    Candle c;
    while (feed.next(c))
    {
        std::cout << c.timestamp << " | " << c.close << "\n";
    }

    return 0;
}
