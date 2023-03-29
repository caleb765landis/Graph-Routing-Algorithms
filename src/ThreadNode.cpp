#include "ThreadNode.h"

ThreadNode::ThreadNode()
{}

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, std::vector<ThreadNode>* nodes)
: _ID(id), _neighbors(neighbors), _nodes(nodes)
{}

ThreadNode::~ThreadNode()
{}

uint16_t ThreadNode::getID() const
{return this->_ID;}

void ThreadNode::run()
{
    std::cout << "Nodes 0: " << _nodes -> at(0).getID() << std::endl;
    std::cout << "Nodes 1: " << _nodes -> at(1).getID() << std::endl;


    // Set message's sender to this node
    // Set final destination of message to random thread that is not current thread
    // Pick a random threadnode based on neighbors list
    // Set message's transmittor to this node and receiver to chosen neighbor

    // Sleep for random duration
    // Set current time as start of keeping track of how long message is in network
    // Send message to that chosen threadnode's mailbox using _mailbox.mbox_send
    // Call thread_recv on chosen threadnode
}

void ThreadNode::thread_recv(void*)
{
    // Increase threadnode's receive count
    // Increase message's hop count
    // Check if message's final destination is this thread

    // If this is final destination:
        // Determine hop count and time that message has been in network
        // Store these values in lists for finalHopCounts and finalTimeTraveledVals
    
    // If this is not final destination:
        // Cool down for a random time
        // Choose new neighbor who is not one that the message was sent from
        // Change message's transmittor to this node and receiver to chosen neighbor
        // Send message to that chosen threadnode's mailbox using _mailbox.mbox_send
        // Call thread_recv on next threadnode
}