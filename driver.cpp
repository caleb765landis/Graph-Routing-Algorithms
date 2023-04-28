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

struct options{
	unsigned int optionD;
	std::string optionR;
	std::string filename;
};

void analyzeResults(std::vector<ThreadNode> nodes, analysis &results);
void createNodes(std::vector<ThreadNode> &nodes, ThreadGraph graph, unsigned int duration);
void runThreads(std::vector<std::thread*> &t, std::vector<ThreadNode> &nodes, std::string algName);
void joinThreads(std::vector<std::thread*> &threads);
options getOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){

	options parameters = getOptions(argc, argv);

	// output stream for storing the graphs analysis

	// std::ofstream analysisFile;
	// analysisFile.open("Analysis.csv");
	// analysisFile << "Graph, Nodes, Edges, Hops, Node-Edge Ratio, Time, \n";


	ThreadGraph graph(parameters.filename);
	std::vector<ThreadNode> nodes;			// Node objects
	std::vector<std::thread*> threads;		// Threads
	analysis results = {0,0};

	std::cout << "\nTest - " << parameters.filename 
			<< " - duration - " << parameters.optionD << "s"
			<< " - Algorithm - " << parameters.optionR << std::endl;

	createNodes(nodes, graph, parameters.optionD);
	runThreads(threads, nodes, parameters.optionR);
	joinThreads(threads);
	analyzeResults(nodes, results);

	// print the totals for the graph and reset the static counting variables for the
	// group of threads.
	std::cout 	<< "Graph (" << parameters.filename 
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

options getOptions(int argc, char *argv[])
{
	options parameters;
	parameters.filename = "./graph/A10.dat";
	parameters.optionD = 10;
	parameters.optionR = "hot";

	for(int i = 0; i < argc; i++){
		std::string temp = argv[i];
		if(temp == "-d"){
			parameters.optionD = std::stoi(argv[i + 1]);
		}
		if(temp == "-r"){
			std::string temp2 = argv[i + 1];
			if(temp2 == "hot" || temp2 == "ant"){
				parameters.optionR = temp2;
			}
			else{
				throw std::invalid_argument("-r option needs to be -r <hot|ant>");
			}
		}
	}

	if(argc > 0){
		std::string temp = argv[argc - 1];
		if(temp.find(".dat") > temp.length() || temp.find("./graph/A") > temp.length()){
			throw std::invalid_argument("filename needs to follow the structure: ./graph/A[10:50].dat");
		}
		else{
			parameters.filename = temp;
		}
	}

	return parameters;
}


