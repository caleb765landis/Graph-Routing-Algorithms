#include "ThreadNode.h"

std::default_random_engine ThreadNode::_generator;
std::mutex ThreadNode::_rand_mtx;

ThreadNode::ThreadNode()
{
}

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes)
    : _ID(id), _neighbors(neighbors), _total_nodes(totalNodes), _numMessagesReceived(0)
{}

ThreadNode::~ThreadNode()
{
}

void ThreadNode::run()
{
    while (true)
    {
        randSleep(50);
        thread_send();
        thread_recv();
    }
}

uint16_t ThreadNode::getID() const
{return this->_ID;}

uint16_t ThreadNode::thread_send()
{
    MessagePacket msg = createMessage();
    const char *dataPtr = msg.getDataStr().c_str();

    // Set current time as start of keeping track of how long message is in network
    msg.timeStart();

    _rand_mtx.lock();
    std::cout << "Node (" << _ID << ") - sending - ";
    std::cout << msg << " - ";
    std::cout << " - to - (" << msg.getReceiver() << ") ";
    std::cout << " - dest - (" << msg.getDestination() << ")\n";
    _rand_mtx.unlock();

    uint16_t bytes = mbox_send(msg.getReceiver(), dataPtr, strlen(dataPtr));

    return bytes;
}

MessagePacket ThreadNode::createMessage()
{
    printTestInfo(_ID, "Create Message");

    uint16_t random_dest = createDestination(0, _total_nodes - 1);
    uint16_t random_recv = getRandomNeighbor(_ID);
    // MessagePacket msg(this->_ID, random_dest, random_recv);
    MessagePacket msg(this->_ID, random_dest, random_recv);
    return msg;
}

void ThreadNode::thread_recv()
{
    // Receive message from mailbox and store it in buffer
    int rbytes = mbox_recv(this->getID(), _buffer, MAX);

    _rand_mtx.lock();
    std::string test = "In thread_recv for node " + std::to_string(getID()) + 
                        ": \nBuffer: " + _buffer + " - Bytes - " + std::to_string(rbytes) + "\n";
   printTestInfo(_ID, test);
    _rand_mtx.unlock();

    // Create a temporary MessagePacket
    MessagePacket temp(_buffer);

    // Increase threadnode's receive count
    _numMessagesReceived++;

    // Check if message's final destination is this thread
    // If this is final destination:
    if (temp.getDestination() == this->getID())
    {
        // Stop message timer
        temp.timeStop();

        // Determine hop count and time that message has been in network
        _rand_mtx.lock();
        std::cout << "Hop Count: " << temp.getHopCount() << " - Time Interval: " << temp.getFinalTimeInterval() << std::endl << std::endl;
        std::cout << "/* ----------------------------------- // ----------------------------------- */" << std::endl << std::endl;
        _rand_mtx.unlock();

        // Store these values in lists for finalHopCounts and finalTimeTraveledVals

    // If this is not final destination:
    } else {
        // Cool down for a random time
        this->randCool(50);
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
        std::cout << "Updated dataStr: " << dataStr << std::endl;

        // Send message to that chosen threadnode receiver's mailbox using _mailbox.mbox_send
        const char *dataPtr = dataStr.c_str();
        mbox_send(receiver, dataPtr, strlen(dataPtr));

        // // Call thread_recv on chosen threadnode receiver
        // _nodes->at(receiver).thread_recv();
    } // end if
} // end thread_recv

void ThreadNode::randSleep(double mean)
{
    int randNumber = (int)(rand_exponential(mean) * 1000);
    // _rand_mtx.lock();
    // std::cout << "Random - " << randNumber << std::endl;
    // _rand_mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::randCool(double mean)
{
    int randNumber = (int)(rand_exponential(mean) * 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

uint16_t ThreadNode::passPotato(uint16_t transmittor, uint16_t destination){
    printTestInfo(_ID, "passPotato");

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
    // get a random number from a uniform distribution in the range
    // of 0 and the number of neighbors that to this Node
    // this will be the neighbor's index
    uint16_t nborIndex = rand_uniform(0, _neighbors.size() - 1);
    uint16_t nbor;

    // if the neighbor at index "nborIndex" was the last sender of the the
    // message then get a neighbor "nbor"
    if(_neighbors.at(nborIndex) == prevSender){
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

double ThreadNode::rand_exponential(double mean) const
{
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
    std::uniform_int_distribution<uint16_t> uniDistro(min, max);
    // create a uniform distribution between the min and max

    _rand_mtx.lock();
    // get a random number from that distribution based on
    // the static generator
    uint16_t randNumber = uniDistro(_generator);
    _rand_mtx.unlock();

    return randNumber;
}

void ThreadNode::printTestInfo(uint16_t id, std::string note) const
{
    _rand_mtx.lock();
    std::cout << "\t\t" << id << " - " << note << std::endl;
    _rand_mtx.unlock();
}