#include "MessagePacket.h"

MessagePacket::MessagePacket(uint16_t sendr, uint16_t dest, uint16_t rcvr)
: _transmittor(sendr), _receiver(rcvr), _sender(sendr), _destination(dest), _hopCount(0)
{
    timeStart();

    std::cout << "\nIN MESSAGE PACKET: " << getTransmittor() << std::endl;
    std::cout << "Receiver: "<< getReceiver() << std::endl;
    std::cout << "Sender: "<< getSender() << std::endl;
    std::cout << "Destination: "<< getDestination() << std::endl;
}

MessagePacket::MessagePacket(const MessagePacket& otherPacket){
    copyMessagePacket(otherPacket);
}

MessagePacket::MessagePacket()
: _transmittor(0), _receiver(0), _sender(0), _destination(0), _hopCount(0)
{}

MessagePacket::~MessagePacket()
{}

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

void MessagePacket::setStartTime(timeval t) {
    this->_startTime = t;
}
timeval MessagePacket::getStartTime() const{
    return this->_startTime;
}

void MessagePacket::setFinalTime(timeval t) {
    this-> _finalTime = t;
}
timeval MessagePacket::getFinalTime() const{
    return this->_finalTime;
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