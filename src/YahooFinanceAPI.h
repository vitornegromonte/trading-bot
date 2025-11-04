#ifndef YAHOOFINANCEAPI_H
#define YAHOOFINANCEAPI_H

#include "ExchangeAPI.h"
#include "interval.h"
#include <string>
#include <ctime>

// Implementation of the ExchangeAPI that consumes public Yahoo Finance endpoints
// using the v7/finance/download API with support for intervals and date ranges
class YahooFinanceAPI : public ExchangeAPI {
public:
    YahooFinanceAPI();
    ~YahooFinanceAPI() override;

    // ExchangeAPI interface - fetch_klines usando limites (compatibilidade com TradingBot)
    std::vector<Candle> fetch_klines(const std::string& symbol,
                                     const std::string& interval,
                                     int limit) override;

    // Define the data interval (daily, weekly, monthly)
    void set_interval(Interval interval);
    
    // Returns candles for a specific date range
    std::vector<Candle> get_ticker_data(const std::string& ticker, 
                                       const std::string& start_date, 
                                       const std::string& end_date);
    
    // Download CSV data and return the filename
    std::string download_ticker_csv(const std::string& ticker, 
                                   const std::string& start_date, 
                                   const std::string& end_date);

private:
    std::string _base_url;
    Interval _interval;

    std::string build_url(const std::string& ticker, 
                         const std::string& start_date,
                         const std::string& end_date);
    
    bool string_replace(std::string& str, 
                       const std::string& from,
                       const std::string& to);
    
    std::string timestamp_from_string(const std::string& date);
    
    void download_file(const std::string& url, const std::string& filename);
    
    std::vector<Candle> parse_csv_to_candles(const std::string& filename);
};

#endif // YAHOOFINANCEAPI_H
