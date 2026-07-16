#ifndef _TIME_H
#define _TIME_H

#include <stddef.h>

typedef long time_t;    // Raw seconds since 1970. Standard tick-tock.
typedef long clock_t;   // CPU cycles. How fast the processor is speedrunning.

struct tm {
    int tm_sec;   // Seconds (0 to 60, just in case of a random leap second)
    int tm_min;   // Minutes (0 to 59)
    int tm_hour;  // Hour (0 to 23, 24-hour style)
    int tm_mday;  // Day of the month (1 to 31)
    int tm_mon;   // Month index (0 is Jan, because arrays start at 0)
    int tm_year;  // Years since 1900 (add 1900 to get the real year)
    int tm_wday;  // Day of the week (0 is Sunday, lazy day)
    int tm_yday;  // Day of the year (0 to 365)
    int tm_isdst; // Daylight savings flag (set to -1 if you have no clue)
    int tm_UNIX;  // Days since the Unix clock started ticking (Jan 1, 1970)
};

#endif
