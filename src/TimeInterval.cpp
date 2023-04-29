#include "TimeInterval.h"

std::mutex TimeInterval::_time_mutex;

TimeInterval::TimeInterval(){}

void TimeInterval::start()
{
        _time_mutex.lock();
        gettimeofday(&this->start_time, NULL);
        _time_mutex.unlock();
}

void TimeInterval::stop()
{
        _time_mutex.lock();
        gettimeofday(&this->end_time, NULL);
        _time_mutex.unlock();
}

void TimeInterval::chronoStart()
{
        _time_mutex.lock();
        _start_time = high_resolution_clock::now();
        _time_mutex.unlock();
}

void TimeInterval::chronoStop()
{
        _time_mutex.lock();
        _end_time = high_resolution_clock::now();
        _time_mutex.unlock();
}

double TimeInterval::GetInterval()
{
        double t = (double)(end_time.tv_sec - start_time.tv_sec);

        return t;
}

double TimeInterval::getChronoInterval()
{
        auto t = duration_cast<milliseconds>(_end_time - _start_time);

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

time_point<high_resolution_clock> TimeInterval::getNow()
{
        _time_mutex.lock();
        auto now = high_resolution_clock::now();
        _time_mutex.unlock();

        return now;
}
