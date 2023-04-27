
#ifndef _NODE_H
#define _NODE_H

#include <stdint.h>
#include <cstdlib>
// #include <thread>
#include <chrono>
#include <vector>
#include <map>
// #include <algorithm>
// #include "mbox.h"
#include "MessagePacket.h"

class Node
{
	public:
		Node();
		Node(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes);
		// Node(uint16_t id, std::map<uint16_t, std::pair<double, std::chrono::time_point<std::chrono::high_resolution_clock>> edges, uint16_t totalNodes);
		Node(const Node& other);
		virtual ~Node() = default;

		uint16_t getID() const;
		std::vector<uint16_t> getNbors() const;
		uint16_t getTotalNodes() const;
		unsigned int getHopCount() const;
		double getTotalTime() const;


	protected:
		unsigned int _total_hops;
		double _total_time;

	private:
		uint16_t _ID;
		uint16_t _total_nodes;
		std::vector<uint16_t> _neighbors;
};

#endif