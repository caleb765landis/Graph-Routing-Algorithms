
#ifndef _THREAD_NODE_H
#define _THREAD_NODE_H

#include <thread>
#include <vector>
#include <stdint.h>
#include "mbox.h"
#include "MessagePacket.h"

#define MAX 1024

class ThreadNode
{
	public:
		ThreadNode();
		ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, std::vector<ThreadNode>* nodes);
		~ThreadNode();

		uint16_t getID() const;

		void run();
		void thread_recv(void*);
	private:
		MessagePacket _msg;
		uint16_t _ID;
		std::vector<uint16_t> _neighbors;
		std::vector<ThreadNode>* _nodes;
		char _buffer[MAX];
};

#endif