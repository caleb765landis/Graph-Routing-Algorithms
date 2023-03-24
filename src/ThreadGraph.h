#ifndef _THREAD_GRAPH_H
#define _THREAD_GRAPH_H

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <stdint.h>


class ThreadGraph
{
	private:
		uint16_t numNodes;
		uint16_t numEdges;
		typedef std::vector<uint16_t> neighbors;
		std::map<uint16_t, neighbors> graph;

	public:
		ThreadGraph(std::string);

		uint16_t getNumNodes() const;
		uint16_t getNumEdges() const;
		neighbors getNeighbors(uint16_t) const;
};

#endif