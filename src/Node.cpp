#include "Node.h"


Node::Node()
    : _ID(-1), _neighbors(0), _total_nodes(0), _total_hops(0), _total_time(0)
{}

Node::Node(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes)
    : _ID(id), _neighbors(neighbors), _total_nodes(totalNodes), _total_hops(0), _total_time(0)
{}

Node::Node(uint16_t id, std::map<uint16_t, double> edges, uint16_t totalNodes)
    : _ID(id), _total_nodes(totalNodes), _total_hops(0), _total_time(0)
{}

Node::Node(const Node& other)
    : _ID(other.getID()), _neighbors(other.getNbors()), _total_nodes(other.getTotalNodes()),
    _total_hops(other.getHopCount()), _total_time(other.getTotalTime()) 
{}

std::vector<uint16_t> Node::getNbors() const{
    return _neighbors;
}

uint16_t Node::getTotalNodes() const{
    return _total_nodes;
}

unsigned int Node::getHopCount() const
{
    return _total_hops;
}

double Node::getTotalTime() const
{
    return _total_time;
}

uint16_t Node::getID() const
{
    return this->_ID;
}


// uint16_t Node::passPotato(uint16_t transmittor, uint16_t destination)
// {
//     // printTestInfo(_ID, "passPotato");
//     std::vector<uint16_t>::const_iterator neighbor;
//     uint16_t random_receiver = getID();

//     // if the final destination is in the neighbors of this
//     // node then we can send it directly to it's destination
//     // without running a random number
//     for(neighbor = getNbors().begin(); neighbor < getNbors().end(); neighbor++){
//         if(*neighbor == destination){
//             random_receiver = *neighbor;
//             break;
//         }
//     }

//     // if the destination is not in the path of neighbors then the
//     // random receiver should still be this nodes ID and will then
//     // find a random receiver for the message
//     if(random_receiver == this->getID()){
//         // find a random neighbor and set equal to destination
//         random_receiver = getRandomNeighbor(transmittor);
//     }

//     return random_receiver;
// }

// void Node::thread_recv()
// {
//     // printTestInfo(_ID, "thread_recv");
 
//     // If mailbox is empty do not receive anything
//     if(mbox_empty(getID()))
//         return;
    
//     int rbytes = mbox_recv(this->getID(), &_buffer, MAX);

//     // Create a temporary MessagePacket
//     MessagePacket temp(_buffer);

//     // Check if message's final destination is this thread
//     // If this is final destination:
//     if (temp.getDestination() == this->getID())
//     {
//         printTestInfo(getID(), "Reached Destination", temp.getSender(), temp.getTransmittor(), temp.getReceiver(), temp.getDestination());
//         recordMessage(temp);
//         incrMsgRecieved(1);

//     // If this is not final destination:
//     } else {
//         // Cool down for a random time
//         this->randCool(COOL);
//         temp.incHopCount();

//         // Choose new neighbor as a receiver to pass the message that was not meant for
// 		// this node. New neighbor must not be the previous sender
//         uint16_t from = temp.getTransmittor();		// previous sender
//         uint16_t dest = temp.getDestination();		// final destination
//         uint16_t receiver = passPotato(from, dest);
//         temp.setReceiver(receiver);

//         // Change message's transmittor to this node
//         temp.setTransmittor(this->getID());

//         // Get updated message data
//         std::string dataStr = temp.getDataStr();

//         printTestInfo(getID(), "Pass Potato", temp.getSender(), from, receiver, dest);

//         // Send message to that chosen Node receiver's mailbox using _mailbox.mbox_send
//         const char *dataPtr = dataStr.c_str();
//         mbox_send(receiver, dataPtr, strlen(dataPtr));
//     } // end if
// } // end thread_recv

// void Node::randSleep(double mean)
// {
//     // printTestInfo(_ID, "randSleep");
//     int randNumber = (int)(rand_exponential(mean) * 1000);
//     std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
// }

// void Node::randCool(double mean)
// {
//     // printTestInfo(_ID, "randCool");
//     int randNumber = (int)(rand_exponential(mean) * 1000);
//     std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
// }

// void Node::incrMsgSent(unsigned int incr)
// {
//     _thread_mtx.lock();
//     this->_messages_sent += incr;
//     _thread_mtx.unlock();
// }

// void Node::incrMsgRecieved(unsigned int incr)
// {
//     _thread_mtx.lock();
//     this->_messages_recieved += incr;
//     _thread_mtx.unlock();
// }

// void Node::recordMessage(MessagePacket msg)
// {
//     // Stop message timer
//     _thread_mtx.lock();
//     msg.timeStop();
//     _thread_mtx.unlock();

//     _total_hops += msg.getHopCount();
//     _total_time += msg.getFinalTimeInterval();

// }

// bool Node::canSend() const
// {
//     bool noMoreSends;

//     _thread_mtx.lock();
//     noMoreSends = _messages_sent <= getMaxMsgs();
//     _thread_mtx.unlock();

//     return noMoreSends;
// }

// void Node::printTestInfo(uint16_t id, std::string action, uint16_t sender, uint16_t trans, uint16_t recv, uint16_t dest) const
// {
//     _stream_mtx.lock();
//     std::cout << "Thread - "<< id << " - " << action 
//             << " - Sender - " << sender << " - Transmittor - " << trans
//             << " - Receiver - " << recv << " - Dest - " << dest 
//             << std::endl;
//     _stream_mtx.unlock();
// }

// void Node::printRunInfo() const
// {
// /* Code written by leemes, user: 592323 published by stack overflow
//     Ref: https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf*/

//     // if(_ID == 0){
//     //     _count_mtx.lock();
//     //     float recvProgress = (float)_messages_recieved / _messages_sent;
//     //     int barWidth = 70;
//     //     _count_mtx.unlock();

//     //     // while (recvProgress < 1.0) {

//     //     std::cout << "Messages Received: [";
//     //     int pos = barWidth * recvProgress;

//     //     _stream_mtx.lock();
//     //     for (int i = 0; i < barWidth; ++i) {
//     //         if (i < pos) std::cout << "=";
//     //         else if (i == pos) std::cout << ">";
//     //         else std::cout << " ";
//     //     }
//     //     std::cout << "] " << int(recvProgress * 100.0) << " %\r";
//     //     //std::cout.clear();
//     //     std::cout.flush();

//     //     if(int(recvProgress*100) == 100)
//     //         std::cout << std::endl; // << "--------------- Progress Done --------------------" << std::endl;

//     //     _stream_mtx.unlock();

//     // }
// }

// bool Node::hasReceivedAllMsgs() const
// {
//     bool allMsgsReceived;

//     _thread_mtx.lock();
//     allMsgsReceived = _messages_recieved != _messages_sent;
//     _thread_mtx.unlock();

//     return allMsgsReceived;
// }

// MessagePacket Node::createMessage()
// {
//     uint16_t random_dest = createDestination(0, _total_nodes - 1);
//     uint16_t random_recv = getRandomNeighbor(_ID, random_dest);
//     MessagePacket msg(this->_ID, random_dest, random_recv);

//     // Set current time as start of keeping track of how long message is in network
//     _thread_mtx.lock();
//     msg.timeStart();
//     _thread_mtx.unlock();

//     return msg;
// }

// uint16_t Node::createDestination(uint16_t min, uint16_t max) const
// {
//     // printTestInfo(_ID, "createDestination");

//     // This should only be used when creating a message will find
//     // The only error checking is to see if the random number is
//     // the current ID
//     // 
//     // get a random number from a uniform distribution in the range
//     // of min to max which should be 0 and total number of nodes in
//     // the graph - 1
//     _thread_mtx.lock();

//     uint16_t destination = RandomNodes::rand_uniform(min, max, _generator);
//     if(destination == this -> _ID)
//         destination = RandomNodes::rand_uniform(min, max, _generator);

//     _thread_mtx.unlock();
    
//     return destination;
// }


// void Node::run()
// {
//     do {
//         Node::randSleep(SLEEP);
//         if(_send_flag){
//             thread_send();
//         }
//         else{
//             printRunInfo();
//         }

//         thread_recv();

//         _send_flag = canSend();
//         _recv_flag = hasReceivedAllMsgs();
 
//     }    while (_send_flag || _recv_flag);

//     // prms.set_value({getHopCount(), getTotalTime()});
// }

// uint16_t Node::thread_send()
// {
//     // if(!_send_flag)
//     //     return -1;

//     // printTestInfo(_ID, "thread_send");
//     MessagePacket msg = createMessage();
//     std::string m = msg.getDataStr();


//     const char *dataPtr = m.c_str();
//     printTestInfo(getID(), "Sending", getID(), getID(), msg.getReceiver(), msg.getDestination());
//     uint16_t bytes = mbox_send(msg.getReceiver(), dataPtr, strlen(dataPtr));

//     incrMsgSent(1);

//     return bytes;
// }


// uint16_t Node::getRandomNeighbor(uint16_t prevSender, uint16_t destination) const
// {
//     uint16_t random_recv;
//     if(std::find(_neighbors.begin(), _neighbors.end(), destination) != _neighbors.end()) {
//         for(auto& n : _neighbors)
//             if(n == destination)
//                 random_recv = n;
//     }
//     else {
//         random_recv = RandomNodes::getRandomNeighbor(prevSender, _neighbors, _generator);
//     }

//     return random_recv;
// }
