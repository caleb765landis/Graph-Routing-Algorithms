#include "ThreadNode.h"

std::default_random_engine ThreadNode::_generator;
std::mutex ThreadNode::_thread_mtx;
std::mutex ThreadNode::_stream_mtx;

unsigned int ThreadNode::_messages_sent = 0;
unsigned int ThreadNode::_messages_recieved = 0;

ThreadNode::ThreadNode()
{}

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes, double duration)
	: _send_flag(true), _recv_flag(true), _duration(duration), Node(id, neighbors, totalNodes)
{
    // seed the generator only once
    if(id == 0)
        _generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

ThreadNode::ThreadNode(uint16_t id, std::map<uint16_t, double> edges, uint16_t totalNodes, double duration)
	: _send_flag(true), _recv_flag(true), Node(id, edges, totalNodes)
{
    // seed the generator only once
    if(id == 0)
        _generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

}

ThreadNode::ThreadNode(const ThreadNode& other)
	: _send_flag(other.getSendFlag()), _recv_flag(other.getRecvFlag()), Node(other)
{}

ThreadNode::~ThreadNode() 
{}

// void ThreadNode::run(std::promise<std::pair<unsigned int, double>> & prms)
void ThreadNode::run()
{
    _thread_mtx.lock();
    _thread_start_time.chronoStart();
    _end_time = _thread_start_time.projectedEnd(_duration);
    _thread_mtx.unlock();

    bool times_up = false;

    do {
        ThreadNode::randSleep(SLEEP);

        _recv_flag = hasReceivedAllMsgs();

        if(!times_up){
            _thread_mtx.lock();
            times_up = _end_time <= high_resolution_clock::now();
            _thread_mtx.unlock();

            thread_send(); 
        }

        thread_recv();
        if(times_up)
            printTestInfo(getID(), "running....", _recv_flag, -1, times_up, -1);

    } while (!times_up || _recv_flag);
}

uint16_t ThreadNode::passPotato(uint16_t transmittor, uint16_t destination)
{
    printTestInfo(getID(), "passPotato", -1, -1, -1, -1);
    // printTestInfo(_ID, "passPotato");
    // std::vector<uint16_t>::const_iterator neighbor;
    // uint16_t random_receiver = getID();

    // // if the final destination is in the neighbors of this
    // // node then we can send it directly to it's destination
    // // without running a random number
    // for(neighbor = getNbors().begin(); neighbor < getNbors().end(); neighbor++){
    //     if(*neighbor == destination){
    //         random_receiver = *neighbor;
    //         break;
    //     }
    // }

    // // if the destination is not in the path of neighbors then the
    // // random receiver should still be this nodes ID and will then
    // // find a random receiver for the message
    // if(random_receiver == this->getID()){
    //     // find a random neighbor and set equal to destination
    //     random_receiver = getRandomNeighbor(transmittor, destination);
    // }

    return getRandomNeighbor(transmittor, destination);
}

uint16_t ThreadNode::thread_send()
{
    // if(!_send_flag)
    //     return -1;

    // printTestInfo(_ID, "thread_send");
    MessagePacket msg = createMessage();
    std::string m = msg.getDataStr();


    const char *dataPtr = m.c_str();
    printTestInfo(getID(), "Sending", getID(), getID(), msg.getReceiver(), msg.getDestination());
    uint16_t bytes = mbox_send(msg.getReceiver(), dataPtr, strlen(dataPtr));

    incrMsgSent(1);

    return bytes;
}

MessagePacket ThreadNode::createMessage()
{
    // printTestInfo(getID(), "createMessage", -1, -1, -1, -1);
    uint16_t random_dest = createDestination(0, getTotalNodes() - 1);
    uint16_t random_recv = getRandomNeighbor(getID(), random_dest);
    MessagePacket msg(getID(), random_dest, random_recv);

    // Set current time as start of keeping track of how long message is in network
    _thread_mtx.lock();
    msg.timeStart();
    _thread_mtx.unlock();

    return msg;
}

uint16_t ThreadNode::createDestination(uint16_t min, uint16_t max) const
{
    // printTestInfo(getID(), "createDestination", -1, -1, -1, -1);

    // This should only be used when creating a message will find
    // The only error checking is to see if the random number is
    // the current ID
    // 
    // get a random number from a uniform distribution in the range
    // of min to max which should be 0 and total number of nodes in
    // the graph - 1
    _thread_mtx.lock();

    uint16_t destination = RandomNodes::rand_uniform(min, max, _generator);
    if(destination == getID())
        destination = RandomNodes::rand_uniform(min, max, _generator);

    _thread_mtx.unlock();
    
    return destination;
}

uint16_t ThreadNode::getRandomNeighbor(uint16_t prevSender, uint16_t destination) const
{
    // printTestInfo(getID(), "getRandomNeighbor", -1, -1, -1, -1);
    uint16_t random_recv = getID();
    std::vector<uint16_t>::const_iterator neighbor;
    for(neighbor = getNbors().begin(); neighbor < getNbors().end(); neighbor++){
        if(*neighbor == destination){
            random_recv = *neighbor;
            break;
        }
    }

    if(random_recv == this->getID()){
        // find a random neighbor and set equal to destination
        random_recv = RandomNodes::getRandomNeighbor(prevSender, getNbors(), _generator);
    }

    return random_recv;
}

void ThreadNode::thread_recv()
{
    // printTestInfo(_ID, "thread_recv");
 
    // If mailbox is empty do not receive anything
    if(mbox_empty(getID()))
        return;
    
    int rbytes = mbox_recv(this->getID(), &_buffer, MAX);

    // Create a temporary MessagePacket
    MessagePacket temp(_buffer);

    // Check if message's final destination is this thread
    // If this is final destination:
    if (temp.getDestination() == this->getID())
    {
        printTestInfo(getID(), "Reached Destination", temp.getSender(), temp.getTransmittor(), temp.getReceiver(), temp.getDestination());
        recordMessage(temp);
        incrMsgRecieved(1);

    // If this is not final destination:
    } else {
        // Cool down for a random time
        this->randCool(COOL);
        temp.incHopCount();

        // Choose new neighbor as a receiver to pass the message that was not meant for
		// this node. New neighbor must not be the previous sender
        uint16_t from = temp.getTransmittor();		// previous sender
        uint16_t dest = temp.getDestination();		// final destination
        uint16_t receiver = passPotato(from, dest);
        temp.setReceiver(receiver);

        // Change message's transmittor to this node
        temp.setTransmittor(this->getID());

        // Get updated message data
        std::string dataStr = temp.getDataStr();

        printTestInfo(getID(), "Pass Potato", temp.getSender(), from, receiver, dest);

        // Send message to that chosen ThreadNode receiver's mailbox using _mailbox.mbox_send
        const char *dataPtr = dataStr.c_str();
        mbox_send(receiver, dataPtr, strlen(dataPtr));
    } // end if
} // end thread_recv

void ThreadNode::incrMsgSent(unsigned int incr)
{
    // printTestInfo(getID(), "incrMsgSent", -1, -1, -1, -1);
    _thread_mtx.lock();
    this->_messages_sent += incr;
    _thread_mtx.unlock();
}

void ThreadNode::incrMsgRecieved(unsigned int incr)
{
    // printTestInfo(getID(), "incrMsgReceived", -1, -1, -1, -1);
    _thread_mtx.lock();
    this->_messages_recieved += incr;
    _thread_mtx.unlock();
}

void ThreadNode::randSleep(double mean)
{
    // printTestInfo(getID(), "randSleep", -1, -1, -1, -1);

    _thread_mtx.lock();
    int randNumber = (int)(RandomNodes::rand_exponential(mean, _generator) * 1000);
    _thread_mtx.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::randCool(double mean)
{
    // printTestInfo(getID(), "randCool", -1, -1, -1, -1);

    _thread_mtx.lock();
    int randNumber = (int)(RandomNodes::rand_exponential(mean, _generator) * 1000);
    _thread_mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::recordMessage(MessagePacket msg)
{
    // printTestInfo(getID(), "recordMessages", -1, -1, -1, -1);
    // Stop message timer
    _thread_mtx.lock();
    msg.timeStop();
    _thread_mtx.unlock();

    _total_hops += msg.getHopCount();
    _total_time += msg.getFinalTimeInterval();

}

// bool ThreadNode::canSend() const
// {
//     bool noMoreSends;

//     _thread_mtx.lock();
//     noMoreSends = _messages_sent <= getMaxMsgs();
//     _thread_mtx.unlock();

//     return noMoreSends;
// }

bool ThreadNode::hasReceivedAllMsgs() const
{
    // printTestInfo(getID(), "hasReceivedAllMsgs", -1, -1, -1, -1);
    bool allMsgsReceived;

    _thread_mtx.lock();
    allMsgsReceived = _messages_recieved != _messages_sent;
    _thread_mtx.unlock();

    return allMsgsReceived;
}

void ThreadNode::printTestInfo(uint16_t id, std::string action, uint16_t sender, uint16_t trans, uint16_t recv, uint16_t dest) const
{
    _thread_mtx.lock();
    std::cout << "Thread - "<< id << " - " << action 
            << " - Sender - " << sender << " - Transmittor - " << trans
            << " - Receiver - " << recv << " - Dest - " << dest 
            << std::endl;
    _thread_mtx.unlock();
}

bool ThreadNode::getSendFlag() const
{
    return this->_send_flag;
}

bool ThreadNode::getRecvFlag() const
{
    return this->_recv_flag;
}