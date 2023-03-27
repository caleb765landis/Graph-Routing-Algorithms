#include "MessagePacket.h"

MessagePacket::MessagePacket(uint16_t sendr, uint16_t dest, uint16_t rcvr)
: _transmittor(sendr), _receiver(rcvr), _sender(sendr), _destination(dest), _hopCount(0)
{
    timeStart();

    std::cout << getTransmittor() << std::endl;
    std::cout << getReceiver() << std::endl;
    std::cout << getSender() << std::endl;
    std::cout << getDestination() << std::endl;
}

MessagePacket::~MessagePacket()
{}

void MessagePacket::setTransmittor(uint16_t trsmtr)
{}

uint16_t MessagePacket::getTransmittor() const
{return _transmittor;}

uint16_t MessagePacket::getTransmittor()
{
    return _transmittor;
}

void MessagePacket::setReceiver(uint16_t rcvr)
{}

uint16_t MessagePacket::getReceiver() const
{return _receiver;}

void MessagePacket::setSender(uint16_t sndr)
{}

uint16_t MessagePacket::getSender() const
{return _sender;}

void MessagePacket::setDestination(uint16_t dest)
{}

uint16_t MessagePacket::getDestination() const
{return _destination;}

void MessagePacket::incHopCount()
{}

uint16_t MessagePacket::getHopCount() const
{return _hopCount;}

void MessagePacket::timeStart()
{
    timeInterval.start();
    _startTime = timeInterval.start_time;
}

void MessagePacket::timeStop()
{}

double MessagePacket::getFinalTimeInterval() const
{}