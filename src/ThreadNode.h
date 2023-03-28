
#ifndef _THREAD_NODE_H
#define _THREAD_NODE_H

#include <thread>
#include <vector>
#include <stdint.h>
#include "mbox.h"
#include "MessagePacket.h"

#define MAX 1024

/*
	// Create graph from data file
	ThreadGraph graph("graph/A10.dat");

	std::cout << "Nodes: " << graph.getNumNodes() << std::endl;
	std::cout << "Edges: " << graph.getNumEdges() << std::endl;

	std::vector<uint16_t>::iterator it;

	// get the neighbors/edges of node zero "0"
	std::vector<uint16_t> temp = graph.getNeighbors(0);

	// print node zero's neighbors/edges
	std::cout << "0 --> ";
	for(it = temp.begin(); it != temp.end(); it++){
		std:: cout << *it << "-";
	}
	std::cout << std::endl;
*/
class ThreadNode
{
	public:
		ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, MessagePacket msg);
		~ThreadNode();

		uint16_t getID() const;

		void run();
		void thread_recv(void*);
	private:
		MessagePacket msg;
		uint16_t ID;
		std::vector<uint16_t> neighbors;
		char buffer[MAX];
};

#endif