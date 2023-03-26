#include <iostream>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <string.h>
#include "src/mbox.h"
#include "src/ThreadGraph.h"
#include "src/mbox.h"
#include "src/ThreadGraph.h"

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

	// get the neighbors/edges of node zero "0"
	std::vector<uint16_t> temp = graph.getNeighbors(0);

	// print node zero's neighbors/edges
	std::cout << "0 --> ";
	for(it = temp.begin(); it != temp.end(); it++){
		std:: cout << *it << "-";
	}
	std::cout << std::endl;


	//all of this code is for testing it can be deleted

	
	// char buffer[MAX];
	// int numBytes = 0;
	// const char* msg = "hello, I am Michael";
	
	// all of this code is for testing it can be deleted
	
	char buffer[MAX];
	int numBytes = 0;
	const char* msgStr = "hello, I am Michael";

	mbox_send(1, msgStr, strlen(msgStr)+1);
	int rbytes = mbox_recv(1, buffer, MAX);

	// Message msg = {1, 2, 1, 2, 0};

	// std::thread a(mbox_send, 2, msgStr, strlen(msgStr));
	// //std::condition_variable cv;

	// std::thread c(mbox_send, 2, "world", strlen(msg));

	// std::thread b(mbox_recv, 1, buffer, MAX);
	// std::thread d(mbox_recv, 2, buffer, MAX);


	// a.join();
	// b.join();
	// c.join();

	std::cout << "Buffer: " << buffer << " - Bytes - " << sizeof(msgStr) << std::endl;

	



	return 0;
}