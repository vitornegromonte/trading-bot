#ifndef INTERVAL_H
#define INTERVAL_H

#include <string>

enum Interval 
{
    DAILY,
    WEEKLY,
    MONTHLY
};

static const std::string IntervalAPIValues[] = { "1d", "1wk", "1mo" };

inline std::string get_api_interval_value(Interval interval)
{
    return IntervalAPIValues[interval];
}

#endif // INTERVAL_H
