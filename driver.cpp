#include <iostream>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <string.h>
#include <typeinfo>
#include "src/mbox.h"
#include "src/ThreadGraph.h"
#include "src/mbox.h"
#include "src/ThreadGraph.h"

#define MAX 1024

struct Message
{
	/* data */
	uint16_t trsmtr;
	uint16_t rcvr;
	uint16_t sendr;
	uint16_t dest;
	uint16_t hCount;
	std::string content;
};

int main(){
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

	/* ----------------------------------- // ----------------------------------- */

	// Create N number of working threadnodes where N is number of nodes in graph
	// Each threadnode will have a thread that runs the threadnode's run method
	// This method will begin the random message writing and passing

	// ? Should there be a list of threadnodes so we can give each one acces to this list for message passing?
	// ? The only problem I have with my implementation is figuring out how to get a reference to the other threadnodes

	// join threads once all processes have finished

	// Go through each threadnode and report thread activity
	// Thread activity includes number of nodes in graph, statistical analysis of travel time, and hop count

	/* ----------------------------------- // ----------------------------------- */
	
	// test code, keep for now
	
	char buffer[MAX];
	const char* msgStr = "hello, I am Michael";

	mbox_send(1, msgStr, strlen(msgStr)+1);
	int rbytes = mbox_recv(1, buffer, MAX);


	// std::thread a(mbox_send, 2, msgStr, strlen(msgStr));
	// std::thread b(mbox_recv, 2, buffer, MAX);

	// a.join();
	// b.join();

	std::cout << "Buffer: " << buffer << " - Bytes - " << sizeof(msgStr) << std::endl;

	// Message msg = {1, 2, 1, 2, 0, "Test"};
	// const Message* msgPtr = &msg;
	// mbox_send(3, msgPtr, sizeof(msg));
	// int rbytes2 = mbox_recv(3, buffer, MAX);
	// std::cout << "Buffer: " << buffer << " - Bytes - " << sizeof(msgStr) << std::endl;

	return 0;
}
