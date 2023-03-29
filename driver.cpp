#include <iostream>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <string.h>
#include <typeinfo>
#include "src/ThreadGraph.h"
#include "src/ThreadNode.h"
#include "src/MessagePacket.h"
#include "src/mbox.h"

#define MAX 1024

int main(){


	// Read from graph data file and store in ThreadGraph
	ThreadGraph graph("graph/A10.dat");

	std::cout << "Nodes: " << graph.getNumNodes() << std::endl;
	std::cout << "Edges: " << graph.getNumEdges() << std::endl;

	std::vector<uint16_t>::iterator it;

	// get the neighbors/edges of node zero "0"
	std::vector<uint16_t> temp = graph.getNeighbors(0);

	// print node zero's neighbors/edges
	std::cout << "0 --> ";
	for (it = temp.begin(); it != temp.end(); it++)
	{
		std::cout << *it << "-";
	}
	std::cout << std::endl << std::endl;

	/* ----------------------------------- // ----------------------------------- */

	// Create N number of working threadnodes where N is number of nodes in graph
	std::vector<ThreadNode> nodes;

	ThreadNode n1(0, graph.getNeighbors(0), &nodes);
	nodes.push_back(n1);

	ThreadNode n2(1, graph.getNeighbors(1), &nodes);
	nodes.push_back(n2);

	n1.run();
	n2.run();

	// Each threadnode will have a thread that runs the threadnode's run method
	// This method will begin the random message writing and passing
	
	// join threads once all processes have finished

	// Go through each threadnode and report thread activity
	// Thread activity includes number of nodes in graph, statistical analysis of travel time, and hop count

	/* ----------------------------------- // ----------------------------------- */
	
	// test code, keep for now
	
	char buffer[MAX];
	// const char* msgStr = "hello, I am Michael";

	// mbox_send(1, msgStr, strlen(msgStr)+1);
	// int rbytes = mbox_recv(1, buffer, MAX);

	MessagePacket msgPacket(0, 2, 2);
	std::string msgPcktDataStr = msgPacket.getDataStr();
	const char *msgPcktDataPtr = msgPcktDataStr.c_str();
	std::cout << "Data String: " << msgPcktDataStr << std::endl;

	mbox_send(2, msgPcktDataPtr, strlen(msgPcktDataPtr));
	int rbytes = mbox_recv(2, buffer, MAX);

	// std::thread a(mbox_send, 2, msgStr, strlen(msgStr));
	// std::thread b(mbox_recv, 2, buffer, MAX);

	// a.join();
	// b.join();

	std::cout << "Buffer: " << buffer << " - Bytes - " << rbytes << std::endl;

	return 0;
}
