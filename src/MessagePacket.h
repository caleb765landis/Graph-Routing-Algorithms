#ifndef _MESSAGE_PACKET_H_
#define _MESSAGE_PACKET_H_

#include <thread>
#include <vector>
#include <stdint.h>
#include <sys/time.h>
#include <cstddef>
#include <sstream>
#include <iostream>
#include <string.h>
#include "mbox.h"
#include "TimeInterval.h"

class MessagePacket
{
public:
    // also sets hop count to 0, transmittor to sendr, and initializes startTime
    MessagePacket(uint16_t sendr, uint16_t dest, uint16_t rcvr);
    MessagePacket(std::string dataStr);
    MessagePacket(const MessagePacket& otherPacket);
    MessagePacket();
    ~MessagePacket();

    std::string getDataStr();
    void setDataStr(std::string dataStr);

    void setTransmittor(uint16_t trsmtr);
    uint16_t getTransmittor() const;
    uint16_t getTransmittor();

    void setReceiver(uint16_t rcvr);
    uint16_t getReceiver() const;

    void setSender(uint16_t sndr);
    uint16_t getSender() const;

    void setDestination(uint16_t dest);
    uint16_t getDestination() const;

    void incHopCount();
    uint16_t getHopCount() const;
    void setHopCount(uint16_t hpcount);

    void timeStart(); // sets start time to keep track of how long message is in network
    void timeStop(); // sets final time to how long since start time has passed

    void setStartTime(double t);
    double getStartTime() const;
    
    void setFinalTime(double t);
    double getFinalTime() const;

    void setTimeInterval(TimeInterval t);
    TimeInterval getTimeInterval() const;

    // stops timer if finalTime is 0. returns total time message was in network
    // difference of stop time and start time
    double getFinalTimeInterval(); 

    MessagePacket operator=(const MessagePacket& other);

private:
    uint16_t _transmittor; // threadnode that last sent the message
    uint16_t _receiver; // threadnode that last recieved the message
    uint16_t _sender; // original sender of message
    uint16_t _destination; // threadnode the message needs to get to
    uint16_t _hopCount; // number of passes between nodes
    timeval _startTime; // time message was created. determines time spend in network
    timeval _finalTime; // time message reached final destination
    TimeInterval timeInterval; // Time interval object for keeping track of time

    void copyMessagePacket(const MessagePacket& other);
};

#endif