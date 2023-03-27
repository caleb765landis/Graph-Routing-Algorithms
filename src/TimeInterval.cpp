#include "TimeInterval.h"

TimeInterval::TimeInterval(){}

void TimeInterval::start()
{
        gettimeofday(&this->start_time, NULL);
}

void TimeInterval::stop()
{
        gettimeofday(&this->end_time, NULL);
}

double TimeInterval::GetInterval()
{
        double t =(double)(end_time.tv_sec-start_time.tv_sec)*1000000.0+(double)(end_time.tv_usec-start_time.tv_usec);  // in micro-sec

//      t = t / 1000000.0               // in sec

        return t;
}

