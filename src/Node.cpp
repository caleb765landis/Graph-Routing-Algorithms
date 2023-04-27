#include "Node.h"


Node::Node()
    : _ID(-1), _neighbors(0), _total_nodes(0), _total_hops(0), _total_time(0)
{}

Node::Node(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes)
    : _ID(id), _neighbors(neighbors), _total_nodes(totalNodes), _total_hops(0), _total_time(0)
{
    // std::cout << id << ": ";
    // for(auto &nbor : getNbors()){
    //     std::cout << nbor << " - ";
    // }
    // std::cout << "end" << std::endl;
}

// Node::Node(uint16_t id, std::map<uint16_t, std::pair<double, std::chrono::time_point<high_resolution_clock>> edges, uint16_t totalNodes)
//     : _ID(id), _total_nodes(totalNodes), _total_hops(0), _total_time(0)
// {}

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
