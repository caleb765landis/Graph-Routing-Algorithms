
#ifndef _THREAD_NODE_H
#define _THREAD_NODE_H

#include <thread>
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

class ThreadNode : public std::thread
{
	public:
		ThreadNode();
		~ThreadNode();

		void run();
	private:
		Message msg;
		char buffer[MAX];
};

#endif