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
	ThreadGraph graph("graph/A49.dat");
	std::vector<ThreadNode> nodes;
	std::vector<std::thread> threads;

	// the total number of messages allowed to be sent by all threads
	// before cooling down and finishing at the same time
	//
	// later we will use a for loop or while loop to cycle through a 
	// range of max_messages to collect statistical data on the threads
	unsigned int max_messages = 15;


	// create the number of nodes given by the graph and store them in
	// a vector
	// std::cout << "Nodes: " << graph.getNumNodes() << std::endl;
	// std::cout << "Edges: " << graph.getNumEdges() << std::endl;
	for (uint16_t i = 0; i < graph.getNumNodes(); i++)
	{
		// std::cout << i << " - " << graph.nborsToString(i) << std::endl;
		ThreadNode temp(i, graph.getNeighbors(i), graph.getNumNodes(), max_messages);
		nodes.push_back(temp);
	}


	// Use a thread for each of the nodes to use the nodes run function.
	std::vector<ThreadNode>::iterator it;
	for(it = nodes.begin(); it < nodes.end(); it++){
		std::thread t(&ThreadNode::run, &(*it));
		threads.push_back(std::move(t));
	}

	// std::this_thread::sleep_for(std::chrono::milliseconds(500));
	// join the threads
	std::vector<std::thread>::iterator th_it;
	for(th_it = threads.begin(); th_it < threads.end(); th_it++){
		if(th_it->joinable()){
			th_it->join();
		}
		else{
			// need to wait
		}
		
	}



	// std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}
