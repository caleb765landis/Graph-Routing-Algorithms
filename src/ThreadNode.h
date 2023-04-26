#ifndef _THREAD_NODE_H
#define _THREAD_NODE_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "Node.h"
#include "RandomNodes.h"


#define MAX 1024
#define COOL 50
#define INCR_PHEROMONE 0.75
#define EXECUTION_CYCLE 500
#define POWER_COEFF 2
#define DILUTION_HALF_LIFE 50


using namespace std::chrono;

class ThreadNode : public Node
{
	public:
		ThreadNode();
		ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes, unsigned int duration);
		ThreadNode(uint16_t id, std::map<uint16_t, double> edges, uint16_t totalNodes, unsigned int duration);
		ThreadNode(const ThreadNode& other);
		~ThreadNode();

		void run(std::string algName);
	
		// unsigned int getMaxMsgs() const;
		// unsigned int getHopCount() const;
		// double getTotalTime() const;

	private:
		TimeInterval _thread_start_time;
		static time_point<high_resolution_clock> _thread_start_t;
		time_point<high_resolution_clock> _end_time;
		unsigned int _duration;

		char _buffer[MAX];
		static bool _stopRecieving;

		static int _messages_sent;
		static int _messages_recieved;

		static std::default_random_engine _generator;
		static std::mutex _thread_mtx;
		static std::condition_variable _thread_cv;

		uint16_t passPotato(uint16_t transmittor, uint16_t destination);

		/*	TODO these are some of the required pheromone functions not developed*/
		uint16_t findTrail(uint16_t transmitt, uint16_t destination);
		void updatePheromone();
		void dilutePheromone();

		uint16_t thread_send(MessagePacket msg);
		void thread_recv();
		void receive();

		// function pointer for choosing how to pass the message
		// will replace the two functions above
		// 
		// typedef uint16_t(*passingAlgorithm)(uint16_t, uint16_t);
		// uint16_t thread_send(passingAlgorithm alg);
		// void thread_recv(passingAlgorithm alg);

		void incrMsgSent(unsigned int incr);
		void incrMsgRecieved(unsigned int incr);
		void recordMessage(MessagePacket msg);

		uint16_t getRandomNeighbor(uint16_t prevSender, uint16_t destingation) const;
		uint16_t createDestination(uint16_t min, uint16_t max) const;
		MessagePacket createMessage();

		// bool canSend() const;
		bool hasReceivedAllMsgs() const;

		void randSleep(double mean);
		void randCool(double mean);

		void printTestInfo(uint16_t id, std::string action, uint16_t sender, uint16_t trans, uint16_t recv, uint16_t destination) const;
};

#endif