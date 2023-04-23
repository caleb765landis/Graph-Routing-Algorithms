
#ifndef _NODE_H
#define _NODE_H

#include <stdint.h>
#include <cstdlib>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <algorithm>
#include "mbox.h"
#include "MessagePacket.h"
#include "RandomNodes.h"

class Node
{
	public:
		Node();
		Node(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes);
		Node(uint16_t id, std::map<uint16_t, double> edges, uint16_t totalNodes);
		Node(const Node& other);
		virtual ~Node() = default;

		uint16_t getID() const;
		std::vector<uint16_t> getNbors() const;
		uint16_t getTotalNodes() const;
		// unsigned int getMaxMsgs() const;
		unsigned int getHopCount() const;
		double getTotalTime() const;

		// virtual void run(std::promise<std::pair<unsigned int, double>> & prms) = 0;
		// virtual void run();

	protected:
		// void thread_recv();
		// uint16_t thread_send();

		// uint16_t passPotato(uint16_t trans, uint16_t dest);
		// void randSleep(double mean);
		// void randCool(double mean);

		// void incrMsgSent(unsigned int incr);
		// void incrMsgRecieved(unsigned int incr);
		// void recordMessage(MessagePacket msg);

		// bool canSend() const;
		// bool hasReceivedAllMsgs() const;

		// double rand_exponential(double mean) const;
		// uint16_t rand_uniform(uint16_t min, uint16_t max) const;

		// uint16_t getRandomNeighbor(uint16_t prevSender, uint16_t destingation) const;
		// uint16_t createDestination(uint16_t min, uint16_t max) const;
		// MessagePacket createMessage();

		unsigned int _total_hops;
		double _total_time;

	private:
		uint16_t _ID;
		uint16_t _total_nodes;
		std::vector<uint16_t> _neighbors;
		std::map<uint16_t, double> edges;
};

#endif