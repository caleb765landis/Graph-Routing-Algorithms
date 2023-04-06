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
	unsigned int MAX_MESSAGES = 1000;
	std::ofstream graphFile;
	graphFile.open("Analysis.csv");
	graphFile << "Graph, Nodes, Edges, Hops, Time, \n";


	unsigned int graphNumber = 10;
	while(graphNumber <= 50){
		std::string gFileName = "graph/A" + std::to_string(graphNumber) + ".dat";
		ThreadGraph graph(gFileName);

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
		std::cout << "\nTest - " << gFileName << " - Messages - " << MAX_MESSAGES << std::endl;


		// create the number of nodes given by the graph and store them in
		// a vector
		// std::cout << "Nodes: " << graph.getNumNodes() << std::endl;
		// std::cout << "Edges: " << graph.getNumEdges() << std::endl;
		for (uint16_t i = 0; i < graph.getNumNodes(); i++)
		{
			// std::cout << i << " - " << graph.nborsToString(i) << std::endl;
			ThreadNode temp(i, graph.getNeighbors(i), graph.getNumNodes(), MAX_MESSAGES);
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
				threads[i].join();
		}


		// loop for storing the values for the total hopcount and total time
		// also prints each nodes hopcount and time to completion
		unsigned int totalHops = 0;
		double totalTime = 0;
		for(int i = 0; i < graph.getNumNodes(); i++){
			std::pair<unsigned int, double> a = ftrs[i].get();
			totalHops += a.first;
			totalTime += a.second;
			std::cout << "Node (" << i << ") - Hop Count - " << a.first << " - Time - " << a.second << std::endl;
		}

		// print the totals for the graph and reset the static counting variables for the
		// group of threads.
		std::cout << "Graph (A" << std::to_string(graphNumber) << ") - Total Hops - " << totalHops << " - Total Time - " << totalTime << std::endl;
		
		graphFile << "A" + std::to_string(graphNumber) << ", "
					<< graph.getNumNodes() << ", "
					<< graph.getNumEdges() << ", "
					<< totalHops << ", " 
					<< totalTime << ", \n";
		ThreadNode::_messages_sent = 0;
		ThreadNode::_messages_recieved = 0;
		graphNumber++;

	}

	graphFile.close();
	MAX_MESSAGES += 5;


	return 0;
}
