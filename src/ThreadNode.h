
#ifndef _THREAD_NODE_H
#define _THREAD_NODE_H

#include <thread>
#include <future>
#include <vector>
#include <random>
#include <mutex>
#include <stdint.h>
#include <cstdlib>
#include "mbox.h"
#include "MessagePacket.h"

#define MAX 1024
#define SLEEP 50
#define COOL 50



class ThreadNode
{
	public:
		ThreadNode();
		ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes, unsigned int max);
		ThreadNode(const ThreadNode& other);
		~ThreadNode();

		uint16_t getID() const;
		std::vector<uint16_t> getNbors() const;
		uint16_t getTotNodes() const;
		unsigned int getMaxMsgs() const;
		unsigned int getHopCount() const;
		double getTotalTime() const;
		//std::promise<std::pair<unsigned int, double>> getPromise() const;

		void run(std::promise<std::pair<unsigned int, double>> & prms);
		// void start_thread();

	private:
		uint16_t _ID;
		uint16_t _total_nodes;
		MessagePacket _msg;
		// std::thread* node_thread;
		std::vector<uint16_t> _neighbors;

		static std::default_random_engine _generator;
		static std::mutex _rand_mtx;
		static std::mutex _count_mtx;
		static std::mutex _stream_mtx;
		static std::mutex _time_mtx;

		char _buffer[MAX];
		static unsigned int _messages_sent;
		static unsigned int _messages_recieved;
		const unsigned int MAX_MESSAGES;
		unsigned int _total_hops;
		double _total_time;
		bool _send_flag = true;
		bool _recv_flag = true;

		double rand_exponential(double mean) const;
		uint16_t rand_uniform(uint16_t min, uint16_t max) const;

		void thread_recv();
		uint16_t thread_send();

		void randSleep(double mean);
		void randCool(double mean);

		uint16_t passPotato(uint16_t transmittor, uint16_t destination);
		uint16_t getRandomNeighbor(uint16_t prevSender) const;
		uint16_t createDestination(uint16_t min, uint16_t max) const;

		MessagePacket createMessage();


		void printTestInfo(uint16_t id, std::string note)const;
		void printRunInfo() const;
};

#endif