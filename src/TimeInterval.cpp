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

void TimeInterval::chronoStart()
{
        _start_time = high_resolution_clock::now();
}

void TimeInterval::chronoStop()
{
        _end_time = high_resolution_clock::now();
}

double TimeInterval::GetInterval()
{
        double t =(double)(end_time.tv_sec-start_time.tv_sec)*1000000.0+(double)(end_time.tv_usec-start_time.tv_usec);  // in micro-sec
        // t = t / 1000000.0               // in sec

        return t;
}

double TimeInterval::getChronoInterval()
{
        duration<double> t = _end_time - _start_time;

        return t.count();
}

time_point<high_resolution_clock> TimeInterval::projectedEnd(int seconds) const
{
        return this->_start_time + duration<int>(seconds);
}

void TimeInterval::setTimes(double st, double ft)
{
        start_time.tv_sec = st;
        end_time.tv_sec = ft;
}

timeval TimeInterval::getStartTime() const{
        return this->start_time;
}

timeval TimeInterval::getEndTime() const{
        return this->end_time;
}

TimeInterval TimeInterval::operator=(const TimeInterval& other){
        this->start_time = other.getStartTime();
        this->end_time = other.getEndTime();
        return *this;
}
