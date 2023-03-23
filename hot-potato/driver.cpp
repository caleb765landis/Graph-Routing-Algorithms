#include <iostream>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <string.h>
#include "mailbox/mbox.h"

#define MAX 1024

int main(){
	
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

	// std::thread a(mbox_send, 1, msg, strlen(msg));
	// std::condition_variable cv;

	// std::thread c(mbox_send, 2, "world", strlen(msg));

	// std::thread b(mbox_recv, 1, buffer, MAX);


	// a.join();
	// b.join();
	// c.join();

	std::cout << "Buffer: " << buffer << " - Bytes - " << sizeof(msg) << std::endl;





	return 0;
}