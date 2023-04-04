#include "ThreadNode.h"

std::default_random_engine ThreadNode::_generator;
std::mutex ThreadNode::_rand_mtx;
std::mutex ThreadNode::_count_mtx;
std::mutex ThreadNode::_stream_mtx;
std::mutex ThreadNode::_time_mtx;
unsigned int ThreadNode::_messages_sent = 0;
unsigned int ThreadNode::_messages_recieved = 0;

ThreadNode::ThreadNode()
: _ID(-1), _neighbors(0), _total_nodes(0), MAX_MESSAGES(0)
{
}

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes, unsigned int max)
    : _ID(id), _neighbors(neighbors), _total_nodes(totalNodes), MAX_MESSAGES(max), _total_hops(0), _total_time(0)
{}

ThreadNode::ThreadNode(const ThreadNode& other)
    : _ID(other.getID()), _neighbors(other.getNbors()), _total_nodes(other.getTotNodes()), MAX_MESSAGES(other.getMaxMsgs()), _total_hops(other.getHopCount()), _total_time(other.getTotalTime())
{}

ThreadNode::~ThreadNode()
{
    // if(node_thread->joinable()){
    //     node_thread->join();
    //     delete node_thread;
    //     node_thread = nullptr;
    // }
}

// void ThreadNode::start_thread()
// {
//     node_thread = new std::thread(&ThreadNode::run, this);
// }

void ThreadNode::run(std::promise<std::pair<unsigned int, double>> & prms)
{
    do {
        randSleep(SLEEP);
        if(_send_flag){
            thread_send();
        }

        thread_recv();

        _count_mtx.lock();
        _send_flag = _messages_sent <= MAX_MESSAGES;
        _recv_flag = _messages_recieved != _messages_sent;
        _count_mtx.unlock();

        // _stream_mtx.lock();
        // std::cout << _ID << " - Sent - " << _messages_sent << " - Received - " << _messages_recieved << std::endl;
        // _stream_mtx.unlock();
    }    while (_send_flag || _recv_flag);


    std::string hops = "Total Hops: " + std::to_string(_total_hops);
    std::string time = " - Total Time: " + std::to_string(_total_time);
    printTestInfo(_ID, hops + time);

    prms.set_value({_total_hops, _total_time});
}

uint16_t ThreadNode::getID() const
{return this->_ID;}

uint16_t ThreadNode::thread_send()
{
    if(!_send_flag)
        return -1;


    // printTestInfo(_ID, "thread_send");
    MessagePacket msg = createMessage();
    std::string m = msg.getDataStr();
    const char *dataPtr = m.c_str();

    // Set current time as start of keeping track of how long message is in network
    _time_mtx.lock();
    msg.timeStart();
    _time_mtx.unlock();

    std::string test = "Node (" + std::to_string(_ID) + ") - sending - " +
                        m.c_str() + " - to - (" + std::to_string(msg.getReceiver()) +
                        ") - dest - (" + std::to_string(msg.getDestination()) + ")";
    printTestInfo(_ID, test);

    uint16_t bytes = mbox_send(msg.getReceiver(), dataPtr, strlen(dataPtr));

    _count_mtx.lock();
    _messages_sent++;
    _count_mtx.unlock();

    return bytes;
}

MessagePacket ThreadNode::createMessage()
{
    // printTestInfo(_ID, "Create Message");

    uint16_t random_dest = createDestination(0, _total_nodes - 1);
    uint16_t random_recv = getRandomNeighbor(_ID);
    // MessagePacket msg(this->_ID, random_dest, random_recv);
    MessagePacket msg(this->_ID, random_dest, random_recv);
    return msg;
}

void ThreadNode::thread_recv()
{
    // printTestInfo(_ID, "thread_recv");
 
    // If mailbox is empty do not receive anything
    if(mbox_empty(_ID))
        return;
    
    int rbytes = mbox_recv(this->getID(), &_buffer, MAX);


    // Create a temporary MessagePacket
    MessagePacket temp(_buffer);

    // std::string test = "Node (" + std::to_string(_ID) + ") - receiving - " +
    //                     temp.getDataStr() + " - to - (" + std::to_string(temp.getReceiver()) +
    //                     ") - dest - (" + std::to_string(temp.getDestination()) + ")";
    // printTestInfo(_ID, test);

    // Check if message's final destination is this thread
    // If this is final destination:
    if (temp.getDestination() == this->getID())
    {
        // Stop message timer
        _time_mtx.lock();
        temp.timeStop();
        _time_mtx.unlock();

        std::string destinString = "Node (" + std::to_string(_ID) + ") - Reached Destination - " +
                            temp.getDataStr() + " - From - (" + std::to_string(temp.getSender()) +
                            ") - transmitor - (" + std::to_string(temp.getTransmittor()) +
                            ") - hop count - (" + std::to_string(temp.getHopCount()) +
                            ") - time - (" + std::to_string(temp.getFinalTimeInterval()) +")";
        printTestInfo(_ID, destinString);

        _total_hops += temp.getHopCount();
        _total_time += temp.getFinalTimeInterval();
        _count_mtx.lock();
        _messages_recieved++;
        _count_mtx.unlock();

        // TODO THIS IS WHERE WE NEED TO RETAIN THE INFROMATION OF THE FINAL DESTINATION FOR
        // A MESSAGE


    // If this is not final destination:
    } else {
        // Cool down for a random time
        this->randCool(COOL);
        temp.incHopCount();

        // Choose new neighbor as receiver who is not one that the message was sent from
        uint16_t from = temp.getTransmittor();
        uint16_t to = temp.getDestination();
        uint16_t receiver = passPotato(from, to);
        temp.setReceiver(receiver);

        // Change message's transmittor to this node
        temp.setTransmittor(this->getID());

        // Get updated message data
        std::string dataStr = temp.getDataStr();

        std::string pass = "Node (" + std::to_string(_ID) + ") - Pass Potato - " +
                            temp.getDataStr() + " - to - (" + std::to_string(temp.getReceiver()) +
                            ") - dest - (" + std::to_string(temp.getDestination()) + ")";
        printTestInfo(_ID, pass);

        // Send message to that chosen threadnode receiver's mailbox using _mailbox.mbox_send
        const char *dataPtr = dataStr.c_str();
        mbox_send(receiver, dataPtr, strlen(dataPtr));
    } // end if
} // end thread_recv

uint16_t ThreadNode::passPotato(uint16_t transmittor, uint16_t destination)
{
    // printTestInfo(_ID, "passPotato");

    std::vector<uint16_t>::const_iterator neighbor;
    uint16_t random_receiver = this->_ID;

    // if the final destination is in the neighbors of this
    // node then we can send it directly to it's destination
    // without running a random number
    for(neighbor = _neighbors.begin(); neighbor < _neighbors.end(); neighbor++){
        if(*neighbor == destination){
            random_receiver = *neighbor;
            break;
        }
    }

    // if the destination is not in the path of neighbors then the
    // random receiver should still be this nodes ID and will then
    // find a random receiver for the message
    if(random_receiver == this->_ID){
        // find a random neighbor and set equal to destination
        random_receiver = getRandomNeighbor(transmittor);
    }

    return random_receiver;
}

uint16_t ThreadNode::getRandomNeighbor(uint16_t prevSender) const
{
    // printTestInfo(_ID, "getRandomNeighbor");

    // get a random number from a uniform distribution in the range
    // of 0 and the number of neighbors that to this Node
    // this will be the neighbor's index
    uint16_t nborIndex = rand_uniform(0, _neighbors.size() - 1);
    uint16_t nbor;

    // if the neighbor at index "nborIndex" was the last sender of the the
    // message then get a neighbor "nbor"
    if(_neighbors.at(nborIndex) == prevSender){
        if(_neighbors.size() != 1)
            nbor = getRandomNeighbor(prevSender);
    }
    else{
        // if not assign neighbor
        nbor = _neighbors.at(nborIndex);
    }

    return nbor;
}

uint16_t ThreadNode::createDestination(uint16_t min, uint16_t max) const
{
    // printTestInfo(_ID, "createDestination");

    // This should only be used when creating a message will find
    // The only error checking is to see if the random number is
    // the current ID
    // 
    // get a random number from a uniform distribution in the range
    // of min to max which should be 0 and total number of nodes in
    // the graph - 1
    uint16_t destination = rand_uniform(min, max);

    if(destination == this -> _ID)
        destination = rand_uniform(min, max);
    
    return destination;
}

void ThreadNode::randSleep(double mean)
{
    // printTestInfo(_ID, "randSleep");
    int randNumber = (int)(rand_exponential(mean) * 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::randCool(double mean)
{
    // printTestInfo(_ID, "randCool");
    int randNumber = (int)(rand_exponential(mean) * 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

double ThreadNode::rand_exponential(double mean) const
{
    // printTestInfo(_ID, "rand_exp");
    std::exponential_distribution<double> expDistro(mean);
    // create an exponinetial distribution around the mean

    _rand_mtx.lock();
    // get a random number fron that distribution based on
    // the static generator
    double randNumber = expDistro(_generator);
    // std::cout << "Exp Random: " << randNumber << std::endl;
    _rand_mtx.unlock();

    return randNumber;
}

uint16_t ThreadNode::rand_uniform(uint16_t min, uint16_t max) const
{
    // printTestInfo(_ID, "rand_uni");
    std::uniform_int_distribution<uint16_t> uniDistro(min, max);
    // create a uniform distribution between the min and max

    _rand_mtx.lock();
    // get a random number from that distribution based on
    // the static generator
    uint16_t randNumber = uniDistro(_generator);
    _rand_mtx.unlock();

    return randNumber;
}

std::vector<uint16_t> ThreadNode::getNbors() const{
    return _neighbors;
}
uint16_t ThreadNode::getTotNodes() const{
    return _total_nodes;
}
unsigned int ThreadNode::getMaxMsgs() const{
    return MAX_MESSAGES;
}
unsigned int ThreadNode::getHopCount() const{
    return _total_hops;
}
double ThreadNode::getTotalTime() const{
    return _total_time;
}

void ThreadNode::printTestInfo(uint16_t id, std::string note) const
{
    // _stream_mtx.lock();
    // std::cout << "Thread - "<< id << " - " << note << std::endl;
    // _stream_mtx.unlock();
}

void ThreadNode::printRunInfo() const
{

}
