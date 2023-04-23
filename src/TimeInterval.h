#ifndef _TIMEINTREVAL_H_
#define _TIMEINTREVAL_H_

#include <sys/time.h>
#include <chrono>
#include <cstddef>

using namespace std::chrono;

class TimeInterval{
public:
        time_point<high_resolution_clock> _start_time;
        time_point<high_resolution_clock> _end_time;

        timeval start_time;
        timeval end_time;

        TimeInterval();
        void start();
        void chronoStart();
        void stop();
        void chronoStop();

        void setTimes(double st, double ft);
        timeval getStartTime() const;
        timeval getEndTime() const;
        double GetInterval();
        double getChronoInterval();
        time_point<high_resolution_clock> projectedEnd(int seconds) const;

        TimeInterval operator=(const TimeInterval& other);
};

#endif //!defined TimeInterval

