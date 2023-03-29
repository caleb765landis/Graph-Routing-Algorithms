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

struct Message
{
	/* data */
	uint16_t trsmtr;
	uint16_t rcvr;
	uint16_t sendr;
	uint16_t dest;
	uint16_t hCount;
	std::string content;
};

int main(){

	/* ----------------------------------- // ----------------------------------- */

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
	char buffer2[MAX];
	// const char* msgStr = "hello, I am Michael";

	// mbox_send(1, msgStr, strlen(msgStr)+1);
	// int rbytes = mbox_recv(1, buffer, MAX);

	MessagePacket msgPacket(0, 2, 2);
	//const char * msgPcktDataStr = &msgPacket;
	// const char * msgPcktDataStr = msgPacket.getDataStr().data();
	std::cout << "Data String: " << msgPcktDataStr << std::endl;

	// const MessagePacket* msgPcktPtr = &msgPacket;
	// MessagePacket* rcvrPtr;
	// MessagePacket* rcvrPtr = nullptr;

	// std::cout << "\nIN MAIN: \n";
	// std::cout << "msgPacket Addr: " << &msgPacket << std::endl;
	// std::cout << "rcvrPtr Addr: " << rcvrPtr << std::endl;

	// std::cout << "Before sending:" << std::endl;
	// std::cout << msgPcktPtr->getTransmittor() << std::endl;
	// std::cout << msgPcktPtr->getReceiver() << std::endl;
	// std::cout << msgPcktPtr->getSender() << std::endl;
	// std::cout << msgPcktPtr->getDestination() << std::endl;

	mbox_send(2, msgPcktDataStr, sizeof(msgPacket));
	mbox_recv(2, buffer2, MAX);
	//mbox_recv(2, rcvrPtr, MAX);

	MessagePacket mp2(buffer2);

	//rcvrPtr = (MessagePacket*)rcvrPtr;
	// rcvrPtr = (MessagePacket*)buffer2;

	// std::cout << "\nIN MAIN: " << std::endl;
	// std::cout << "After receiving:" << std::endl;
	// std::cout << "rcvrPtr Addr: " << rcvrPtr << std::endl;
	// std::cout << ((MessagePacket*)rcvrPtr) -> getTransmittor() << std::endl;
	// std::cout << ((MessagePacket*)rcvrPtr) -> getReceiver() << std::endl;
	// std::cout << ((MessagePacket*)rcvrPtr) -> getSender() << std::endl;
	// std::cout << ((MessagePacket*)rcvrPtr) -> getDestination() << std::endl;


	// std::thread a(mbox_send, 2, msgStr, strlen(msgStr));
	// std::thread b(mbox_recv, 2, buffer, MAX);

	// a.join();
	// b.join();

	//std::cout << "Buffer: " << buffer << " - Bytes - " << sizeof(msgStr) << std::endl;

	// Message msg = {1, 2, 1, 2, 0, "Test"};
	// const Message* msgPtr = &msg;
	// mbox_send(3, msgPtr, sizeof(msg));
	// int rbytes2 = mbox_recv(3, buffer, MAX);


	std::cout << "Buffer: " << buffer2 << " - Bytes - " << sizeof(buffer2) << std::endl;

	//delete msgPP;

	return 0;
}
