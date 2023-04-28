#include <iostream>
#include <fstream>
#include <thread>
// #include <condition_variable>
#include <string.h>
#include <typeinfo>
// #include <future>
#include "src/ThreadGraph.h"
#include "src/ThreadNode.h"
// #include "src/MessagePacket.h"
// #include "src/mbox.h"

#define MAX 1024

struct analysis{
	unsigned int hops;
	double time;
};

void analyzeResults(std::vector<ThreadNode> nodes, analysis &results);
void createNodes(std::vector<ThreadNode> &nodes, ThreadGraph graph, unsigned int duration);
void runThreads(std::vector<std::thread*> &t, std::vector<ThreadNode> &nodes, std::string algName);
void joinThreads(std::vector<std::thread*> &threads);

int main(int argc, char *argv[]){

	double optionD = 3; 					// <-- TODO get value from main arguments
	// char optionR[] = {'h','o','t'}; 		// <-- TODO get value from main arguments
	char optionR[] = {'a','n','t'}; 		// <-- TODO get value from main arguments
	std::string filename = "./graph/A10.dat";	// <-- TODO get value from main arguments

	// output stream for storing the graphs analysis

	// std::ofstream analysisFile;
	// analysisFile.open("Analysis.csv");
	// analysisFile << "Graph, Nodes, Edges, Hops, Node-Edge Ratio, Time, \n";


	ThreadGraph graph(filename);
	std::vector<ThreadNode> nodes;			// Node objects
	std::vector<std::thread*> threads;		// Threads
	analysis results = {0,0};

	std::cout << "\nTest - " << filename << " - duration - " << optionD << "s"<< std::endl;

	createNodes(nodes, graph, optionD);
	runThreads(threads, nodes, optionR);
	joinThreads(threads);
	analyzeResults(nodes, results);

	// print the totals for the graph and reset the static counting variables for the
	// group of threads.
	std::cout 	<< "Graph (" << filename 
				<< ") - Total Hops - " << results.hops 
				<< " - Total Time - " << results.time << " ms\n";
	
	// analysisFile << filename << ", "
	// 			<< graph.getNumNodes() << ", "
	// 			<< graph.getNumEdges() << ", "
	// 			<< (double)graph.getNumEdges() / graph.getNumNodes() << ", "
	// 			<< results.hops << ", " 
	// 			<< results.time << ", \n";



	// analysisFile.close();


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

void runThreads(std::vector<std::thread*> &threads, std::vector<ThreadNode> &nodes, std::string algorithmName)
{
	// Use a thread for each of the nodes to use the nodes run function.
	std::vector<ThreadNode>::iterator it;
	for(it = nodes.begin(); it != nodes.end(); it++){
		threads.push_back(std::move(new std::thread(&ThreadNode::run, &(*it), algorithmName)));
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

void analyzeResults(std::vector<ThreadNode> nodes, analysis &results)
{
	// reset the results if they are not already set
	results.hops = 0;
	results.time = 0;

	// loop for storing the values for the total hopcount and total time
	// also prints each nodes hopcount and time to completion
	for(int i = 0; i < nodes.size(); i++){
		std::pair<unsigned int, double> p = {nodes[i].getHopCount(), nodes[i].getTotalTime()};
		results.hops += p.first;
		results.time += p.second;
		std::cout << "Node (" << i << ") - Hop Count - " << p.first << " - Time - " << p.second << " ms\n";
	}
}

