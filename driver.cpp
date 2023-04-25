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

analysis analyzeResults(std::vector<ThreadNode> nodes);
void createNodes(std::vector<ThreadNode> &nodes, ThreadGraph graph, unsigned int duration);
void runThreads(std::vector<std::thread*> &t, std::vector<ThreadNode> &nodes);
void joinThreads(std::vector<std::thread*> &threads);

int main(int argc, char *argv[]){

	double optionD = 3; 					// <-- TODO get value from main arguments
	char optionR[] = {'h','o','t'}; 		// <-- TODO get value from main arguments
	std::string filename = "graph/A10.dat";	// <-- TODO get value from main arguments

	// output stream for storing the graphs analysis
	std::ofstream analysisFile;
	analysisFile.open("Analysis.csv");
	analysisFile << "Graph, Nodes, Edges, Hops, Node-Edge Ratio, Time, \n";

	/*
		Beginning work on deciding which algorithm to use but I am not
		familiar with polymorphism and would like to be ambiguous for
		selecting which class to use.

		If you all have an idea on how to implement this let me know.
	*/
	if(strcmp(optionR, "hot") == 0){

	}
	else if(strcmp(optionR, "ant") == 0){

	}

	ThreadGraph graph(filename);

	std::vector<ThreadNode> nodes;			// Node objects
	std::vector<std::thread*> threads;		// Threads

	std::cout << "\nTest - " << filename << " - duration - " << optionD << "s"<< std::endl;

	createNodes(nodes, graph, optionD);
	// these two functions work with both types of nodes
	runThreads(threads, nodes);

	joinThreads(threads);
	analysis results = analyzeResults(nodes);

	// print the totals for the graph and reset the static counting variables for the
	// group of threads.
	std::cout 	<< "Graph (" << filename 
				<< ") - Total Hops - " << results.hops 
				<< " - Total Time - " << results.time << std::endl;
	
	analysisFile << filename << ", "
				<< graph.getNumNodes() << ", "
				<< graph.getNumEdges() << ", "
				<< (double)graph.getNumEdges() / graph.getNumNodes() << ", "
				<< results.hops << ", " 
				<< results.time << ", \n";



	analysisFile.close();


	return 0;
}

void createNodes(std::vector<ThreadNode> &nodes, ThreadGraph graph, unsigned int duration)
{
	for (uint16_t i = 0; i < graph.getNumNodes(); i++)
	{
		ThreadNode temp(i, graph.getNeighbors(i), graph.getNumNodes(), duration);
		nodes.push_back(temp);
	}
}

void runThreads(std::vector<std::thread*> &threads, std::vector<ThreadNode> &nodes)
{
	// Use a thread for each of the nodes to use the nodes run function.
	std::vector<ThreadNode>::iterator it;
	for(it = nodes.begin(); it != nodes.end(); it++){
		threads.push_back(std::move(new std::thread(&ThreadNode::run, &(*it))));
	}
}

void joinThreads(std::vector<std::thread*> &threads)
{
	// Makes main wait for each of the nodes to be joinable 
	for(int i = 0; i < threads.size(); i++){
		if(threads[i]->joinable())
			threads[i]->join();
			delete threads[i];
			threads[i] = nullptr;
	}

}

analysis analyzeResults(std::vector<ThreadNode> nodes)
{
	// loop for storing the values for the total hopcount and total time
	// also prints each nodes hopcount and time to completion
	analysis a;
	for(int i = 0; i < nodes.size(); i++){
		std::pair<unsigned int, double> p = {nodes[i].getHopCount(), nodes[i].getTotalTime()};
		a.hops += p.first;
		a.time += p.second;
		std::cout << "Node (" << i << ") - Hop Count - " << a.hops << " - Time - " << a.time << std::endl;
	}

	return a;
}

