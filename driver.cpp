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
	std::vector<ThreadNode> nodes;

	// the total number of messages allowed to be sent by all threads
	// before cooling down and finishing at the same time
	//
	// later we will use a for loop or while loop to cycle through a 
	// range of max_messages to collect statistical data on the threads
	unsigned int max_messages = 50;		

	// create the number of nodes given by the graph and store them in
	// a vector
	for (uint16_t i = 0; i < graph.getNumNodes(); i++)
	{
		ThreadNode temp(i, graph.getNeighbors(i), graph.getNumNodes(), max_messages);
		nodes.push_back(temp);
	}

	// Each threadnode will have a thread that runs the threadnode's run method
	// Cycle through the vecor and start each thread
	std::vector<ThreadNode>::iterator it;
	for(it = nodes.begin(); it < nodes.end(); it++){
		it->start_thread();
	}

	return 0;
}
