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
        double GetInterval();
};

#endif //!defined TimeInterval

