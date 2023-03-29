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

	/* ----------------------------------- // ----------------------------------- */

	// Read from graph data file and store in ThreadGraph

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
	// const char* msgStr = "hello, I am Michael";

	// mbox_send(1, msgStr, strlen(msgStr)+1);
	// int rbytes = mbox_recv(1, buffer, MAX);

	MessagePacket msgPacket(0, 2, 2);
	std::string msgPcktDataStr = msgPacket.getDataStr();
	const char *msgPcktDataPtr = msgPcktDataStr.c_str();
	std::cout << "Data String: " << msgPcktDataStr << std::endl;

	mbox_send(2, msgPcktDataPtr, strlen(msgPcktDataPtr));
	int rbytes = mbox_recv(2, buffer, MAX);

	// std::thread a(mbox_send, 2, msgStr, strlen(msgStr));
	// std::thread b(mbox_recv, 2, buffer, MAX);

	// a.join();
	// b.join();

	std::cout << "Buffer: " << buffer << " - Bytes - " << rbytes << std::endl;

	return 0;
}
