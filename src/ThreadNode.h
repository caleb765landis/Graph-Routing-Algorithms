#ifndef _THREAD_NODE_H
#define _THREAD_NODE_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "Node.h"
#include "RandomNodes.h"


#define MAX 1024
#define COOL 50
#define EXECUTION_CYCLE 500

#define INIT_PHEROMONE 2.50
#define INCR_PHEROMONE 7.50
#define POWER_COEFF 2.24
#define DILUTION_HALF_LIFE 1000

using namespace std::chrono;

class ThreadNode : public Node
{
	public:
		ThreadNode();
		ThreadNode(uint16_t id, std::vector<uint16_t> neighbors, uint16_t totalNodes, unsigned int duration);
		ThreadNode(const ThreadNode& other);
		~ThreadNode();

		void run(std::string algName);
		std::map<uint16_t, double> getEdges() const;
		std::map<uint16_t, time_point<high_resolution_clock>> getEdgeTimes() const;
		unsigned int getDuration() const;

	
		// unsigned int getMaxMsgs() const;
		// unsigned int getHopCount() const;
		// double getTotalTime() const;

	private:
		// TimeInterval _thread_start_time;
		// static time_point<high_resolution_clock> _thread_start_t;
		// time_point<high_resolution_clock> _end_time;
		unsigned int _duration;
		std::map<uint16_t, double> _edges;
		std::map<uint16_t, time_point<high_resolution_clock>> _edge_times;
		std::string _algorithmType;
		char _buffer[MAX];

		static bool _stopRecieving;
		static int _messages_sent;
		static int _messages_recieved;
		static std::default_random_engine _generator;
		static std::mutex _thread_mtx;
		static std::condition_variable _thread_cv;

		MessagePacket passPotato(MessagePacket msg);
		MessagePacket moveAnt(MessagePacket msg);

		/*	TODO these are some of the required pheromone functions not developed*/
		void incrPheromone(uint16_t from);
		void dilutePheromones();

		uint16_t thread_send(MessagePacket msg);
		void thread_recv();
		void receive(MessagePacket (ThreadNode::*passAlgorithm)(MessagePacket msg));

		// function pointer for choosing how to pass the message
		// will replace the two functions above
		// 
		// typedef uint16_t(*passingAlgorithm)(uint16_t, uint16_t);
		// uint16_t thread_send(passingAlgorithm alg);
		// void thread_recv(passingAlgorithm alg);

		void incrMsgSent(unsigned int incr);
		void incrMsgRecieved(unsigned int incr);
		void recordMessage(MessagePacket msg);

		uint16_t findTrail(uint16_t prevSender, uint16_t dest);
		uint16_t getRandomNeighbor(uint16_t prevSender, uint16_t dest);
		uint16_t createDestination(uint16_t min, uint16_t max) const;
		MessagePacket createMessage(uint16_t (ThreadNode::*passAlgorithm)(uint16_t prev, uint16_t dest));

		// bool canSend() const;
		bool hasReceivedAllMsgs() const;

		void randSleep(double mean);
		void randCool(double mean);

		void printTestInfo(uint16_t id, std::string action, uint16_t sender, uint16_t trans, uint16_t recv, uint16_t destination) const;
};

#endif