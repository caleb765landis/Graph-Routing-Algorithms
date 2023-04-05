#include <iostream>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <string.h>
#include <typeinfo>
#include <future>
#include "src/ThreadGraph.h"
#include "src/ThreadNode.h"
#include "src/MessagePacket.h"
#include "src/mbox.h"

#define MAX 1024

struct analysis{
	unsigned int hops;
	double time;
};

int main(){
	// Read from graph data file and store in ThreadGraph
	ThreadGraph graph("graph/A10.dat");
	std::vector<ThreadNode> nodes;						// Node objects
	std::vector<std::thread> threads;					// Threads
	std::queue<analysis> final_analysis;
	typedef std::pair<unsigned int, double> threadAnalysis;
	std::map<uint16_t, std::promise<threadAnalysis>> prms;		// threads promise to send hop count and time
	std::map<uint16_t, std::future<threadAnalysis>> ftrs;		// main waits for promises to be fulfilled

	// the total number of messages allowed to be sent by all threads
	// before cooling down and finishing at the same time
	//
	// later we will use a for loop or while loop to cycle through a 
	// range of max_messages to collect statistical data on the threads
	unsigned int max_messages = 50;


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
	uint16_t index = 0;
	std::vector<ThreadNode>::iterator it;
	for(it = nodes.begin(); it != nodes.end(); it++){
		std::thread t(&ThreadNode::run, &(*it), std::ref(prms[index]));
		threads.push_back(std::move(t));
		index++;
	}

	// Makes main wait for each of the nodes to be joinable and collects
	// the hopcount and time information from each threads promise
	for(int i = 0; i < graph.getNumNodes(); i++){
		if(threads[i].joinable())
			ftrs[i] = prms[i].get_future();
			std::pair<unsigned int, double> a = ftrs[i].get();
			std::cout << "Node (" << i << ") - hop count - " << a.first << " - time - " << a.second << std::endl;
			threads[i].join();
	}


	// std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}
