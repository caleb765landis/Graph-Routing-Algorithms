#include "ThreadNode.h"

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, Message msg)
: ID(id), neighbors(neighbors), msg(msg)
{}

ThreadNode::~ThreadNode()
{}

uint16_t ThreadNode::getID() const
{return this->ID;}

// handles checking if this node is the destination for message
// if it is, report hcount and time
// otherwise check if dest is one of neighbors - if it is then pass to that neighbor
// if dest is not one of neighbors, choose random neighbor
void ThreadNode::thread_recv(void*)
{}

void ThreadNode::run()
{}