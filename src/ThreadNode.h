
#ifndef _THREAD_NODE_H
#define _THREAD_NODE_H

#include <thread>
#include <vector>
#include <stdint.h>
#include "mbox.h"

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

class ThreadNode
{
	public:
		ThreadNode();
		~ThreadNode();

		uint16_t getID() const;
		void thread_recv(void*);

		void run();
	private:
		Message msg;
		uint16_t ID;
		std::vector<uint16_t> neighbors;
		char buffer[MAX];
};

#endif