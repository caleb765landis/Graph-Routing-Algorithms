#include "ThreadNode.h"

std::default_random_engine ThreadNode::_generator;
std::mutex ThreadNode::_thread_mtx;
std::condition_variable ThreadNode::_thread_cv;
bool ThreadNode::_stopRecieving;


int ThreadNode::_messages_sent = 0;
int ThreadNode::_messages_recieved = 0;

ThreadNode::ThreadNode()
{}

ThreadNode::ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes, unsigned int duration)
	: _duration(duration), Node(id, neighbors, totalNodes)
{
    // seed the generator, and thread start time only once
    // not sure if we still need the _stopReceiving boolean value or not
    // but I am initializing it to false any way.
    if(id == 0)
    {
        _generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
        // _thread_start_t = high_resolution_clock::now();
        _stopRecieving = false;
    }

    // initiallize the node edges for pheromones
    for(auto &it : neighbors){
        _edges[it] = pow(INIT_PHEROMONE, POWER_COEFF);
        _edge_times[it] = high_resolution_clock::now();
    }
    // Decrementing the _messages_recieved based on the number of nodes created
    // so the independant Receive threads will not finish until the main threads
    // are done
    _messages_recieved--;
}

ThreadNode::ThreadNode(const ThreadNode& other)
	: _duration(other.getDuration()), _edges(other.getEdges()), _edge_times(other.getEdgeTimes()), 
    Node(other)
{
    
}

ThreadNode::~ThreadNode() 
{}

void ThreadNode::run(std::string algName)
{
    // printTestInfo(getID(), "Running...", -1, -1, -1, -1);
    this->_algorithmType = algName;

    _thread_mtx.lock();
    auto start = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    _thread_mtx.unlock();

    auto timer = duration_cast<milliseconds>(now - start);   
    auto end = milliseconds(_duration * 1000);

    // create the reciever thread and pass it the thread_recv function
    // of this node
    //
    std::thread reciever(&ThreadNode::thread_recv, this);


    // time the sending for each thread based on the _duration till "end"
    while(timer <= end) {
        // the running of each send THREAD needs to be staggered by the EXECUTION_CYCLE time
        // by adding the EXECUTION time with the node's ID times the EXECUTION_CYCLE and 
        // the total number of nodes
        //                                                ID * EXECUTION_CYCLE
        //      EXECUTION_SLEEP_TIME = EXECUTION_CYCLE + ---------------------
        //                                                    TOTAL_NODES
        //  EX:
        //                                        0 * EXECUTION_CYCLE
        //      EST(NODE(0)) = EXECUTION_CYCLE + --------------------- = EXECUTION_CYCLE
        //                                            TOTAL_NODES
        //
        std::this_thread::sleep_for(std::chrono::milliseconds(EXECUTION_CYCLE + (getID() * EXECUTION_CYCLE) / getTotalNodes() ) );

        // randSleep is the old Implementation of the HOT_POTATO algorithm
        // randSleep(SLEEP);

        // depending on whichever algorithm chosen we need to either use the
        // getRandom or findTrail methods
        if(algName == "hot"){
            thread_send(createMessage(&ThreadNode::getRandomNeighbor));
            incrMsgSent(1);
        }
        else if(algName == "ant"){
            thread_send(createMessage(&ThreadNode::findTrail));
            incrMsgSent(1);
        }

        _thread_mtx.lock();
        now = high_resolution_clock::now();
        _thread_mtx.unlock();
        timer = duration_cast<milliseconds>(now - start);
    }

    // after the main threads are done we increment the message recieved
    // because at the creation of the node we decremented by the number
    // of threads created so that when we are running the other independent
    // receive thread it starts at a deficit so it could not finish until
    // the sending threads are done
    //
    // Once all main threads have finished the receive threads could then
    // finish afterwards
    incrMsgRecieved(1);

    if(reciever.joinable())
        reciever.join();
}

MessagePacket ThreadNode::passPotato(MessagePacket msg)
{

    msg.incHopCount();

    // Choose new neighbor as a receiver to pass the message that was not meant for
    // this node. New neighbor must not be the previous sender
    uint16_t from = msg.getTransmittor();		// previous sender
    uint16_t dest = msg.getDestination();		// final destination
    uint16_t receiver = getRandomNeighbor(from, dest);
    msg.setReceiver(receiver);
    msg.setTransmittor(getID());

    printTestInfo(getID(), "Pass Potato", msg.getSender(), msg.getTransmittor(), msg.getReceiver(), msg.getDestination());

    return msg;
}

MessagePacket ThreadNode::moveAnt(MessagePacket msg)
{
    // printTestInfo(getID(), "Move Ant", -1, -1, -1, -1);

    msg.incHopCount();
    dilutePheromones();
    incrPheromone(msg.getTransmittor());

    // printTestInfo(getID(), "Move Ant - After INCREMENT", -1, -1, -1, -1);
    // Choose new neighbor as a receiver to pass the message that was not meant for
    // this node. New neighbor must not be the previous sender
    uint16_t from = msg.getTransmittor();		// previous sender
    uint16_t dest = msg.getDestination();		// final destination
    uint16_t receiver = findTrail(from, dest);
    msg.setReceiver(receiver);
    msg.setTransmittor(getID());

    printTestInfo(getID(), "Ant on The Move", msg.getSender(), msg.getTransmittor(), msg.getReceiver(), msg.getDestination());

    return msg;
}

void ThreadNode::incrPheromone(uint16_t from)
{
    // printTestInfo(getID(), "incrPheronome", -1, -1, -1, -1);
    // add to the pheromone based on an ant walking acrosse the edge
    _edges[from] += INCR_PHEROMONE;

    // since an ant just walked on the edge restart the half-life clock
    _thread_mtx.lock();
    _edge_times[from] = high_resolution_clock::now();
    _thread_mtx.unlock();
}

void ThreadNode::dilutePheromones()
{
    // printTestInfo(getID(), "Dilution:", -1, -1, -1, -1);
    for(auto &it : _edges){
        // start is the last time an ant walked on the edge or
        // the last time a message was received from this neighbor
        auto start = _edge_times[it.first];

        _thread_mtx.lock();
        auto now = high_resolution_clock::now();
        _thread_mtx.unlock();

        auto duration = duration_cast<milliseconds>(now - start);
        _thread_mtx.lock();
        // std::cout << "Prior:   key - " << it.first << " - value - " << it.second << " - time - " << duration.count() << std::endl;
        it.second = it.second * pow( 0.5, ((duration.count()/DILUTION_HALF_LIFE)));
        if(it.second < INIT_PHEROMONE)
            it.second = INIT_PHEROMONE; 
        // std::cout << "Diluted: key - " << it.first << " - value - " << it.second << " - time - " << duration.count() << std::endl;
        _thread_mtx.unlock();
    }
}

uint16_t ThreadNode::thread_send(MessagePacket msg)
{
    std::string m = msg.getDataStr();

    // use the mailbox to send the message
    const char *dataPtr = m.c_str();
    uint16_t bytes = mbox_send(msg.getReceiver(), dataPtr, strlen(dataPtr));

    // increment the total amount of messages sent

    return bytes;
}

MessagePacket ThreadNode::createMessage(uint16_t (ThreadNode::*passAlgorithm)(uint16_t prev, uint16_t dest))
{
    // printTestInfo(getID(), "Create Message", -1, -1, -1, -1);
    // create a message and get a random neighbor to send that
    // message to
    uint16_t random_dest = createDestination(0, getTotalNodes() - 1);
    uint16_t random_recv = (*this.*passAlgorithm)(getID(), random_dest);
    MessagePacket msg(getID(), random_dest, random_recv);

    printTestInfo(getID(), "Sending", getID(), getID(), msg.getReceiver(), msg.getDestination());

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
    _thread_mtx.unlock();
    if(destination == getID()){
        _thread_mtx.lock();
        destination = RandomNodes::rand_uniform(min, max, _generator);
        _thread_mtx.unlock();
    }

    
    return destination;
}

uint16_t ThreadNode::getRandomNeighbor(uint16_t prevSender, uint16_t destination)
{
    // printTestInfo(getID(), "getRandomNeighbor", -1, -1, -1, -1);
    
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

uint16_t ThreadNode::findTrail(uint16_t prevSender, uint16_t dest)
{
    /* First check to see if the destination is one of this nodes neighbors **/
    uint16_t antTrail = getID();
    // std::vector<uint16_t>::const_iterator neighbor;
    // std::cout << getNbors() << std::endl;
    for(auto &neighbor : getNbors()) {//= getNbors().begin(); neighbor != getNbors().end(); neighbor++){
        // printTestInfo(getID(), "Not accessing in trail", -1, -1, -1, -1);
        // printTestInfo(getID(), "Trailing .. ", neighbor, dest, antTrail, -1);
        if(neighbor == dest){
            antTrail = neighbor;
            break;
        }
    }


    /* If the random receiver is still this nodes ID then we know the destination
        is not among this nodes neighbors.  Get a randomNeighbor from the random
        nodes class
    */
    if(antTrail == this->getID()){
        // find a random neighbor and set equal to destination
        // printTestInfo(getID(), "Before entering pheronome", -1, -1, -1, -1);
        _thread_mtx.lock();
        antTrail = RandomNodes::getPheromoneNeighbor(prevSender, _edges, POWER_COEFF, _generator);
        _thread_mtx.unlock();
    }

    // printTestInfo(getID(), "After Random Pheromone", -1, -1, -1, -1);
    return antTrail;

}

void ThreadNode::thread_recv()
{
    // printTestInfo(getID(), "Other Thread", -1, -1, -1, -1);

    _thread_mtx.lock();
    bool stopReceiving = ThreadNode::_stopRecieving;
    _thread_mtx.unlock();

    while(!stopReceiving){
        // printTestInfo(getID(), "In loop", -1, -1, -1, -1);
        // try{
        if(_algorithmType == "hot"){
            receive(&ThreadNode::passPotato);
        }
        else if (_algorithmType == "ant")
        {
            receive(&ThreadNode::moveAnt);
        }
        
        // printTestInfo(getID(), "After Receive", -1, -1, -1, -1);
        if(hasReceivedAllMsgs()){
            _thread_mtx.lock();
            ThreadNode::_stopRecieving = true;
            _thread_mtx.unlock();

            stopReceiving = ThreadNode::_stopRecieving;
            
        }
        // } catch (std::exception &e){
        //     _thread_mtx.lock();
        //     std::cout << "Thread - " << getID() << " - thread_recv - " << e.what() << std::endl;
        //     _thread_mtx.unlock();
        // }
    }
} // end thread_recv

void ThreadNode::receive(MessagePacket (ThreadNode::*passAlgorithm)(MessagePacket msg))
{
    // if bytes are 0 then there wasn't anything in the buffer
    // return
    
    int rbytes = mbox_recv(getID(), &_buffer, MAX);
    if(rbytes <= 0)
        return;

    MessagePacket temp(_buffer);


    // Check if message's final destination is this thread
    // If this is final destination:
    if (temp.getDestination() == getID())
    {
        printTestInfo(getID(), "Reached Destination", temp.getSender(), temp.getTransmittor(), temp.getReceiver(), temp.getDestination());
        recordMessage(temp);
        incrMsgRecieved(1);

    // If this is not final destination:
    } else {
        // Cool down for a random time
        randCool(COOL);
        MessagePacket forwardMessage = (*this.*passAlgorithm)(temp);
        thread_send(forwardMessage);
    }
}

void ThreadNode::incrMsgSent(unsigned int incr)
{
    // printTestInfo(getID(), "incrMsgSent", -1, -1, -1, -1);

    // increment the messages by the increement variable
    _thread_mtx.lock();
    this->_messages_sent += incr;
    _thread_mtx.unlock();
}

void ThreadNode::incrMsgRecieved(unsigned int incr)
{
    // printTestInfo(getID(), "incrMsgReceived", -1, -1, -1, -1);

    // increment the messages received by the increment variable
    _thread_mtx.lock();
    this->_messages_recieved += incr;
    _thread_mtx.unlock();
}

void ThreadNode::randSleep(double mean)
{
    // printTestInfo(getID(), "randSleep", -1, -1, -1, -1);

    // choose a random number for the node to sleep
    _thread_mtx.lock();
    int randNumber = (int)(RandomNodes::rand_exponential(mean, _generator) * 1000);
    _thread_mtx.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::randCool(double mean)
{
    // printTestInfo(getID(), "randCool", -1, -1, -1, -1);

    // choose a random number for the node to cool
    _thread_mtx.lock();
    int randNumber = (int)(RandomNodes::rand_exponential(mean, _generator) * 1000);
    _thread_mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(randNumber));
}

void ThreadNode::recordMessage(MessagePacket msg)
{
    // printTestInfo(getID(), "recordMessages", msg.getSender(), msg.getTransmittor(), msg.getReceiver(), msg.getDestination());
    // Stop message timer
    _thread_mtx.lock();
    msg.timeStop();
    _thread_mtx.unlock();

    // record the messages hops and time
    Node::_total_hops += msg.getHopCount();
    _total_time += msg.getFinalTimeInterval();

    if(_algorithmType == "ant"){
        incrPheromone(msg.getTransmittor());
    }
}

bool ThreadNode::hasReceivedAllMsgs() const
{
    bool allMsgsReceived = false;

    // check to see if the number of messages received matches the number
    // of messages sent
    _thread_mtx.lock();
    allMsgsReceived = _messages_recieved >= _messages_sent;
    _thread_mtx.unlock();
    
    // printTestInfo(getID(), "HAS RECEIVED ALL MSGS ", (uint16_t)allMsgsReceived, (uint16_t)_messages_recieved, (uint16_t)_messages_sent, -1);

    return allMsgsReceived;
}

void ThreadNode::printTestInfo(uint16_t id, std::string action, uint16_t sender, uint16_t trans, uint16_t recv, uint16_t dest) const
{
    // print test information for depugging
    _thread_mtx.lock();
    std::cout << "Thread - "<< std::to_string(id) << " - " << action 
            << " - Sender - " << std::to_string(sender) << " - Transmittor - " << std::to_string(trans)
            << " - Receiver - " << std::to_string(recv) << " - Dest - " << std::to_string(dest)
            << " -> (" << std::to_string(_messages_sent) << ":" << std::to_string(_messages_recieved) << ")" << std::endl;
    _thread_mtx.unlock();
}

std::map<uint16_t, double> ThreadNode::getEdges() const
{
    return _edges;
}

std::map<uint16_t, time_point<high_resolution_clock>> ThreadNode::getEdgeTimes() const
{
    return _edge_times;
}

unsigned int ThreadNode::getDuration() const
{
    return _duration;
}

