#include "MessagePacket.h"

MessagePacket::MessagePacket()
: _transmittor(0), _receiver(0), _sender(0), _destination(0), _hopCount(0)
{}

MessagePacket::MessagePacket(uint16_t sendr, uint16_t dest, uint16_t rcvr)
: _transmittor(sendr), _receiver(rcvr), _sender(sendr), _destination(dest), _hopCount(0)
{}

MessagePacket::MessagePacket(std::string dataStr)
{
    setDataStr(dataStr);
}

MessagePacket::MessagePacket(const MessagePacket& otherPacket){
    copyMessagePacket(otherPacket);
}

MessagePacket::~MessagePacket()
{}

void MessagePacket::init(uint16_t sendr, uint16_t dest, uint16_t rcvr)
{
    _transmittor = sendr;
    _receiver = rcvr;
    _sender = sendr;
    _destination = dest;
    _hopCount = 0;
}

std::string MessagePacket::getDataStr() const
{
    std::string dataStr = "";
    std::string currentStr = "";
    std::stringstream ss;
    ss << _transmittor << ',' << _receiver << ',' << _sender << ',' << _destination << ',' << _hopCount << ',';
    while (std::getline(ss, currentStr, ','))
    {
        dataStr += currentStr + ',';
    }

    ss.clear();
    currentStr = "";
    ss << (double)_startTime.tv_sec << ',';
    ss >> currentStr;
    dataStr += currentStr;

    ss.clear();
    currentStr = "";
    ss << (double)_finalTime.tv_sec << ',';
    ss >> currentStr;
    dataStr += currentStr;

    return dataStr;
}

void MessagePacket::setDataStr(std::string dataStr)
{
    std::string trsmtrStr = "";
    std::string rcvrStr = "";
    std::string sndrStr = "";
    std::string destStr = "";
    std::string hCntStr = "";
    std::string sTimeStr = "";
    std::string fTimeStr = "";

    std::stringstream ss;
    ss << dataStr;

    std::getline(ss, trsmtrStr, ',');
    std::getline(ss, rcvrStr, ',');
    std::getline(ss, sndrStr, ',');
    std::getline(ss, destStr, ',');
    std::getline(ss, hCntStr, ',');
    std::getline(ss, sTimeStr, ',');
    std::getline(ss, fTimeStr, ',');

    setTransmittor(std::stoi(trsmtrStr));
    setReceiver(std::stoi(rcvrStr));
    setSender(std::stoi(sndrStr));
    setDestination(std::stoi(destStr));
    setHopCount(std::stoi(hCntStr));
    setStartTime(std::stod(sTimeStr));
    setFinalTime(std::stod(fTimeStr));
    timeInterval.setTimes(getStartTime(), getFinalTime());
}

void MessagePacket::setTransmittor(uint16_t trsmtr)
{_transmittor = trsmtr;}

uint16_t MessagePacket::getTransmittor() const
{return _transmittor;}

uint16_t MessagePacket::getTransmittor()
{return _transmittor;}

void MessagePacket::setReceiver(uint16_t rcvr)
{_receiver = rcvr;}

uint16_t MessagePacket::getReceiver() const
{return _receiver;}

void MessagePacket::setSender(uint16_t sndr)
{_sender = sndr;}

uint16_t MessagePacket::getSender() const
{return _sender;}

void MessagePacket::setDestination(uint16_t dest)
{_destination = dest;}

uint16_t MessagePacket::getDestination() const
{return _destination;}

void MessagePacket::incHopCount()
{_hopCount++;}

uint16_t MessagePacket::getHopCount() const
{return _hopCount;}

void MessagePacket::timeStart()
{
    timeInterval.start();
    _startTime = timeInterval.start_time;
}

void MessagePacket::timeStop()
{
    timeInterval.stop();
    _finalTime = timeInterval.end_time;
}

double MessagePacket::getFinalTimeInterval()
{
    return timeInterval.GetInterval();
}

void MessagePacket::setHopCount(uint16_t hpcount){
    this->_hopCount = hpcount;
}

void MessagePacket::setStartTime(double t) {
    this->_startTime.tv_sec = t;
}
double MessagePacket::getStartTime() const{
    return this->_startTime.tv_sec;
}

void MessagePacket::setFinalTime(double t) {
    this-> _finalTime.tv_sec = t;
}
double MessagePacket::getFinalTime() const{
    return this->_finalTime.tv_sec;
}

void MessagePacket::setTimeInterval(TimeInterval t){
    this->timeInterval = t;
}
TimeInterval MessagePacket::getTimeInterval() const{
    return this->timeInterval;
}

void MessagePacket::copyMessagePacket(const MessagePacket& other){
    setTransmittor(other.getTransmittor());
    setReceiver(other.getReceiver());
    setSender(other.getSender());
    setDestination(other.getDestination());
    setHopCount(other.getHopCount());
    setStartTime(other.getStartTime());
    setFinalTime(other.getFinalTime());
    setTimeInterval(other.getTimeInterval());

}

MessagePacket MessagePacket::operator=(const MessagePacket& other){
    copyMessagePacket(other);
    return *this;
}

std::ostream& operator<<(std::ostream& out, const MessagePacket& packet)
{
    out << packet.getDataStr();
    return out;
}