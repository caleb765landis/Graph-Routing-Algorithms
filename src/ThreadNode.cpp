#include "ThreadNode.h"

std::default_random_engine ThreadNode::_generator;
std::mutex ThreadNode::_thread_mtx;
std::condition_variable ThreadNode::_thread_cv;
time_point<high_resolution_clock> ThreadNode::_thread_start_t;
bool ThreadNode::_stopRecieving;

int ThreadNode::_messages_sent = 0;
int ThreadNode::_messages_recieved = 0;

ThreadNode::ThreadNode()
{}

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes, unsigned int duration)
	: _send_flag(true), _recv_flag(true), _duration(duration), Node(id, neighbors, totalNodes)
{
    // seed the generator only once
    if(id == 0)
    {
        _generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
        _thread_start_t = high_resolution_clock::now();
        _stopRecieving = false;
    }
    _messages_recieved--;
}

ThreadNode::ThreadNode(uint16_t id, std::map<uint16_t, double> edges, uint16_t totalNodes, unsigned int duration)
	: _send_flag(true), _recv_flag(true), _duration(duration), Node(id, edges, totalNodes)
{
    // seed the generator only once
    if(id == 0){
        _generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
        _stopRecieving = false;
    }
    _messages_recieved--;
}

ThreadNode::ThreadNode(const ThreadNode& other)
	: _duration(other._duration), _send_flag(other.getSendFlag()), _recv_flag(other.getRecvFlag()), Node(other)
{}

ThreadNode::~ThreadNode() 
{}

void ThreadNode::run()
{
    std::thread reciever(&ThreadNode::thread_recv, *this);

    auto start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    auto timer = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);   
    auto end = std::chrono::milliseconds(_duration * 1000);

    while(timer <= end) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // randSleep(SLEEP);
        thread_send(createMessage());

        std::lock_guard<std::mutex> lock(_thread_mtx);
        now = std::chrono::high_resolution_clock::now();
        timer = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    }

    _thread_mtx.lock();
    _messages_recieved++;
    _thread_mtx.unlock();        

    if(reciever.joinable())
        reciever.join();
}

uint16_t ThreadNode::passPotato(uint16_t transmittor, uint16_t destination)
{
    printTestInfo(getID(), "passPotato", -1, -1, -1, -1);

    /* TODO can put more code in here for specifically passing a potato
        Whatever we can move from thread_send where we are passing the 
        potato can go here
    */
    return getRandomNeighbor(transmittor, destination);
}

uint16_t ThreadNode::thread_send(MessagePacket msg)
{
    printTestInfo(getID(), "Sending", getID(), getID(), msg.getReceiver(), msg.getDestination());
    std::string m = msg.getDataStr();

    // use the mailbox to send the message
    const char *dataPtr = m.c_str();
    uint16_t bytes = mbox_send(msg.getReceiver(), dataPtr, strlen(dataPtr));

    // increment the total amount of messages sent
    incrMsgSent(1);

    return bytes;
}

MessagePacket ThreadNode::createMessage()
{
    printTestInfo(getID(), "createMessage", -1, -1, -1, -1);
    // create a message and get a random neighbor to send that
    // message to
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
    printTestInfo(getID(), "createDestination", -1, -1, -1, -1);

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
    printTestInfo(getID(), "getRandomNeighbor", -1, -1, -1, -1);
    
    /* First check to see if the destination is one of this nodes neighbors **/
    uint16_t random_recv = getID();
    std::vector<uint16_t>::const_iterator neighbor;
    for(neighbor = getNbors().begin(); neighbor < getNbors().end(); neighbor++){
        if(*neighbor == destination){
            random_recv = *neighbor;
            break;
        }
    }

    /* If the random receiver is still this nodes ID then we know the destination
        is not among this nodes neighbors.  Get a randomNeighbor from the random
        nodes class
    */
    if(random_recv == this->getID()){
        // find a random neighbor and set equal to destination
        random_recv = RandomNodes::getRandomNeighbor(prevSender, getNbors(), _generator);
    }

    return random_recv;
}

void ThreadNode::thread_recv()
{
    printTestInfo(getID(), "Other Thread", _stopRecieving, -1, -1, -1);
    while(!_stopRecieving){
        receive();
        
        if(hasReceivedAllMsgs()){
            std::lock_guard<std::mutex> lock(_thread_mtx);
            _stopRecieving = true;
        }
    }
} // end thread_recv

void ThreadNode::receive()
{
    int rbytes = mbox_recv(this->getID(), &_buffer, MAX);
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
        randCool(COOL);
        temp.incHopCount();

        // Choose new neighbor as a receiver to pass the message that was not meant for
        // this node. New neighbor must not be the previous sender
        uint16_t from = temp.getTransmittor();		// previous sender
        uint16_t dest = temp.getDestination();		// final destination
        uint16_t receiver = passPotato(from, dest);
        temp.setReceiver(receiver);
        temp.setTransmittor(getID());

        thread_send(temp);
    }
}

void ThreadNode::incrMsgSent(unsigned int incr)
{
    printTestInfo(getID(), "incrMsgSent", -1, -1, -1, -1);

    // increment the messages by the increement variable
    _thread_mtx.lock();
    this->_messages_sent += incr;
    _thread_mtx.unlock();
}

void ThreadNode::incrMsgRecieved(unsigned int incr)
{
    printTestInfo(getID(), "incrMsgReceived", -1, -1, -1, -1);

    // increment the messages received by the increment variable
    _thread_mtx.lock();
    this->_messages_recieved += incr;
    _thread_mtx.unlock();
}

void ThreadNode::randSleep(double mean)
{
    printTestInfo(getID(), "randSleep", -1, -1, -1, -1);

    // choose a random number for the node to sleep
    _thread_mtx.lock();
    int randNumber = (int)(RandomNodes::rand_exponential(mean, _generator) * 1000);
    _thread_mtx.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::randCool(double mean)
{
    printTestInfo(getID(), "randCool", -1, -1, -1, -1);

    // choose a random number for the node to cool
    _thread_mtx.lock();
    int randNumber = (int)(RandomNodes::rand_exponential(mean, _generator) * 1000);
    _thread_mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::recordMessage(MessagePacket msg)
{
    printTestInfo(getID(), "recordMessages", msg.getSender(), msg.getTransmittor(), msg.getReceiver(), msg.getDestination());
    // Stop message timer
    _thread_mtx.lock();
    msg.timeStop();
    _thread_mtx.unlock();

    // record the messages hops and time
    _total_hops += msg.getHopCount();
    _total_time += msg.getFinalTimeInterval();
}

bool ThreadNode::hasReceivedAllMsgs() const
{
    printTestInfo(getID(), "hasReceivedAllMsgs", -1, -1, -1, -1);
    bool allMsgsReceived;

    // check to see if the number of messages received matches the number
    // of messages sent
    _thread_mtx.lock();
    allMsgsReceived = _messages_recieved == _messages_sent;
    _thread_mtx.unlock();

    // printTestInfo(getID(), "HAS RECEIVED ALL MSGS ", (uint16_t)allMsgsReceived, (uint16_t)_messages_recieved, (uint16_t)_messages_sent, -1);

    return allMsgsReceived;
}

void ThreadNode::printTestInfo(uint16_t id, std::string action, uint16_t sender, uint16_t trans, uint16_t recv, uint16_t dest) const
{
    // print test information for depugging
    _thread_mtx.lock();
    std::cout << "Thread - "<< id << " - " << action 
            << " - Sender - " << sender << " - Transmittor - " << trans
            << " - Receiver - " << recv << " - Dest - " << dest 
            << " -> (" << _messages_sent << ":" << _messages_recieved << ")" << std::endl;
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