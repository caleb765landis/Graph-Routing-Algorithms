#include <iostream>
// #include <fstream>
// #include <queue>
// #include <map>
// #include <list>
// #include <stack>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <string.h>
#include "mailbox/mbox.h"
#include "ThreadGraph.h"

#include <typeinfo>

#define MAX 1024

struct Message
{
	/* data */
	uint16_t trsmtr;
	uint16_t rcvr;
	uint16_t sendr;
	uint16_t dest;
	uint16_t hCount;

};

int main(){

	ThreadGraph graph("graph/A10.dat");

	std::cout << "Nodes: " << graph.getNumNodes() << std::endl;
	std::cout << "Edges: " << graph.getNumEdges() << std::endl;

	std::vector<uint16_t>::iterator it;
	std::vector<uint16_t> temp = graph.getNeighbors(0);
	std::cout << "0 --> ";
	for(it = temp.begin(); it != temp.end(); it++){
		std:: cout << *it << "-";
	}
	std::cout << std::endl;
	// std::ifstream file;
	// std::queue<std::string> nodeList;

	// try{
	// 	file.open("graph/A10.dat");

	// 	while(!file.eof()){
	// 		int num;
	// 		std::string line;
	// 		std::getline(file, line, '\n');

	// 		if(line != "\n")
	// 			nodeList.push(line);
	// 	}
	// 	file.close();
	// } catch(...){}

	// std::map<uint16_t, std::list<uint16_t>> graph;
	// while(!nodeList.empty()){
	// 	// removes the last element of the queue because of that pesky newline character that
	// 	// getline gives at the end.
	// 	if(nodeList.size() == 1){
	// 		nodeList.pop();
	// 		continue;
	// 	}
	// 	std::string temp = nodeList.front();

	// 	if(temp.at(0) == '#'){
	// 		if(temp.find("nodes") <= temp.length()){
	// 			std::cout << "Nodes: " << temp.substr(temp.find("= ") + 2, (temp.find("= ") + 2) - (temp.length() - 1)) << std::endl;
	// 		}
	// 		else if (temp.find("edges") <= temp.length()){
	// 			std::cout << "Edges: " << temp.substr(temp.find("= ") + 2, (temp.find("= ") + 2) - (temp.length() - 1)) << std::endl;
	// 		}
	// 	}
	// 	else{
	// 		if(temp.find(':') <= temp.length()){
	// 			std::string nodeID = temp.substr(0, temp.find(':'));
	// 			std::string neighbors = temp.substr(temp.find(':') + 1, temp.find(':') - (temp.length() - 1) );
	// 			std::cout << nodeID << " --> " << neighbors << std::endl;
	// 			std::cout << nodeID << " --> ";

	// 			for(int i = 0; i < neighbors.length(); i++){
	// 				if(neighbors.at(i) != ','){
	// 					graph[std::stoi(nodeID)].push_back(static_cast<uint16_t>(neighbors.at(i)));
	// 					std::cout << neighbors.at(i) << " - ";
	// 				}
	// 			}
	// 			std::cout << std::endl << std::endl;
	// 		}
	// 	}
	// 	nodeList.pop();
	// }


	//all of this code is for testing it can be deleted

	
	// char buffer[MAX];
	// int numBytes = 0;
	// const char* msg = "hello, I am Michael";
	
	// all of this code is for testing it can be deleted
	
	char buffer[MAX];
	int numBytes = 0;
	const char* msg = "hello, I am Michael";

	mbox_send(1, msg, strlen(msg)+1);
	int rbytes = mbox_recv(1, buffer, MAX);

	std::ifstream inFile;
	inFile.open("graph/A10.dat");

	// get rid of comment lines
	std::string temp;
	std::getline(inFile, temp);
	std::getline(inFile, temp);

	std::string nodeID = "0";
	std::string neighbors[10];
	while (std::getline(inFile, nodeID, ':'))
	{
		//std::getline(inFile, nodeID, ':');
		std::cout << nodeID << ":";

		int i = 0;
		while (std::getline(inFile, neighbors[i], ','))
		{
			std::cout << neighbors[i] << ',';
		}
		std::cout << std::endl << std::endl;
	} // end while
	inFile.close();

	// std::thread a(mbox_send, 1, &msg, strlen(msg));
	// //std::condition_variable cv;

	// std::thread c(mbox_send, 2, "world", strlen(msg));

	// std::thread b(mbox_recv, 1, buffer, MAX);
	// std::thread d(mbox_recv, 2, buffer, MAX);


	// a.join();
	// b.join();
	// c.join();

	// std::cout << "Buffer: " << buffer << " - Bytes - " << sizeof(msg) << std::endl;

	



	return 0;
}