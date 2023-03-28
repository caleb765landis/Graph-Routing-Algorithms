#ifndef _TIMEINTREVAL_H_
#define _TIMEINTREVAL_H_

#include <sys/time.h>
#include <cstddef>

class TimeInterval{
public:
        timeval start_time;
        timeval end_time;

        TimeInterval();
        void start();
        void stop();

        timeval getStartTime() const;
        timeval getEndTime() const;
        double GetInterval();

        TimeInterval operator=(const TimeInterval& other);
};

#endif //!defined TimeInterval

