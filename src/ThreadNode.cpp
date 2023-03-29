#include "ThreadNode.h"

ThreadNode::ThreadNode()
{
}

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, std::vector<ThreadNode> *nodes)
    : _ID(id), _neighbors(neighbors), _nodes(nodes), _numMessagesReceived(0)
{
}

ThreadNode::~ThreadNode()
{
}

uint16_t ThreadNode::getID() const
{
    return this->_ID;
}

void ThreadNode::run()
{
    // std::cout << "Nodes 0: " << _nodes -> at(0).getID() << std::endl;
    // std::cout << "Nodes 1: " << _nodes -> at(1).getID() << std::endl;

    std::cout << "Node ID: " << this->getID() << std::endl;
    std::cout << "Neighbors: ";
    std::vector<uint16_t>::iterator it;
    for (it = _neighbors.begin(); it != _neighbors.end(); it++)
    {
        std::cout << *it << "-";
    }
    std::cout << std::endl;

    // seed random? probably don't want so it's easier to tell when everything's working

    // Set message's sender to this node
    uint16_t sender = this->getID();

    // Set final destination of message to random thread that is not current thread
    int destination;
    bool keepGoing = true;
    while (keepGoing)
    {
        destination = rand() % _nodes->size();
        if ((uint16_t)destination != this->getID())
        {
            keepGoing = false;
            std::cout << "Random destination: " << destination << std::endl << std::endl;
        }
    }

    // Pick a random threadnode based on neighbors list
    // Set receiver to chosen neighbor
    int receiverLoc = rand() % _neighbors.size();
    uint16_t receiver = _neighbors[receiverLoc];
    std::cout << "Receiver: " << receiver << std::endl;

    // Initialize message packet information
    _msg.init(sender, (uint16_t)destination, receiver);

    // Sleep for random duration
    this->randSleep(5);

    // Set current time as start of keeping track of how long message is in network
    _msg.timeStart();

    // Get message packet data str
    std::string dataStr = _msg.getDataStr();
    std::cout << "DataStr in run: " << dataStr << std::endl << std::endl;

    // Send message to that chosen threadnode receiver's mailbox using _mailbox.mbox_send
    const char *dataPtr = dataStr.c_str();
    mbox_send(receiver, dataPtr, strlen(dataPtr));

    // Call thread_recv on chosen threadnode receiver
    _nodes -> at(receiver).thread_recv();
}

void ThreadNode::thread_recv()
{
    // Receive message from mailbox and store it in buffer
    int rbytes = mbox_recv(this->getID(), _buffer, MAX);

    std::cout << "In thread_recv for node " << this->getID() << ": " << std::endl;
    std::cout << "Buffer: " << _buffer << " - Bytes - " << rbytes << std::endl << std::endl;

    // Create a temporary MessagePacket
    MessagePacket temp(_buffer);

    // Increase threadnode's receive count
    _numMessagesReceived++;

    // Increase message's hop count
    temp.incHopCount();

    // Check if message's final destination is this thread
    // If this is final destination:
    if (temp.getDestination() == this->getID())
    {
        // Stop message timer
        temp.timeStop();

        // Determine hop count and time that message has been in network
        std::cout << "Hop Count: " << temp.getHopCount() << " - Time Interval: " << temp.getFinalTimeInterval() << std::endl << std::endl;

        std::cout << "/* ----------------------------------- // ----------------------------------- */" << std::endl << std::endl;

        // Store these values in lists for finalHopCounts and finalTimeTraveledVals

    // If this is not final destination:
    } else {
        // Cool down for a random time
        this->randCool(0, 1);

        // Choose new neighbor as receiver who is not one that the message was sent from
        uint16_t receiver;
        bool keepGoing = true;
        while (keepGoing)
        {
            int receiverLoc = rand() % _neighbors.size();
            receiver = _neighbors[receiverLoc];
            if (receiver != temp.getTransmittor())
            {
                keepGoing = false;
                temp.setReceiver(receiver);
                std::cout << "New Receiver: " << receiver << std::endl;
            } // end if
        } // end while

        // Change message's transmittor to this node
        temp.setTransmittor(this->getID());

        // Get updated message data
        std::string dataStr = temp.getDataStr();
        std::cout << "Updated dataStr: " << dataStr << std::endl;

        // Send message to that chosen threadnode receiver's mailbox using _mailbox.mbox_send
        const char *dataPtr = dataStr.c_str();
        mbox_send(receiver, dataPtr, strlen(dataPtr));

        // Call thread_recv on chosen threadnode receiver
        _nodes->at(receiver).thread_recv();
    } // end if
} // end thread_recv

void ThreadNode::randSleep(double mean)
{
    // apply rand_exponential algorithm then sleep for that time
    std::cout << "sleeping" << std::endl;
}

void ThreadNode::randCool(double min, double max)
{
    // apply rand_exponential algorithm then sleep for that time
    std::cout << "cooling" << std::endl;
}