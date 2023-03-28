/*
 * mailbox.cc
 * Copyright(c) 2022 Dongsoo S. Kim
 */
#include <iostream>
#include <string.h>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "mbox.h"
#include "MessagePacket.h"



class MailBox {
		struct item {
			int length;
			// MessagePacket* content;
			char* content;
		};

		typedef std::queue<item> mailbox_t;       	// defining a queue of mailboxes to be a type for simple annotation
		std::map<uint16_t,mailbox_t> _mailboxes;  	// the queue of mailboxes mapped to an unsigned integer
		std::map<uint16_t, std::mutex> _mtx;		// parallel map of locks to each mailbox ID
		std::map<uint16_t, std::condition_variable> cvs;
		std::mutex m;								// basic lock for code deletion
		int ID;
	public:
		MailBox() {
		};
		~MailBox();
		bool empty(uint16_t msgID); 
		int  send(uint16_t msgID, const void *packet, int len);
		int  recv(uint16_t msgID, void *packet, int max);
};

static MailBox mailbox;


MailBox::~MailBox()
{
	struct item xtem;
	m.lock(); 
	for (auto x : _mailboxes) {
		while (!x.second.empty()) {
			xtem = x.second.front();
			x.second.pop(); 
			delete [] xtem.content; 
		}
	}
	m.unlock(); 
}

bool MailBox::empty(uint16_t msgID){
	bool isEmpty = false;

	// critical section
	_mtx[msgID].lock();
	if(_mailboxes[msgID].empty() )
		isEmpty = true;
	_mtx[msgID].unlock();
	
	return isEmpty;
}

int MailBox::send(uint16_t msgID, const void *packet, int len){
	// item msg = {len, (MessagePacket *)packet};
	item msg = {len, (char *)packet};
	int numBytes = sizeof(msg);

	const MessagePacket * _packet = (MessagePacket*)packet;
	std::cout << "Packet: " << _packet -> getTransmittor() << std::endl;

	//std::cout << "Packet: " << msg.content->getTransmittor() << std::endl;

	//std::cout << msg.content->getTransmittor() << std::cout;
	//std::cout << (MessagePacket*)packet->getTransmittor() << std::endl;

	// could be critical section
	//mtx.lock();
	//std::cout << "Packet: " << (char *)packet << std::endl;
	// std::cout << "Item: " << msg.content << std::endl;
	std::lock_guard<std::mutex> lg(_mtx[msgID]);
	_mailboxes[msgID].push(msg);
	//mtx.unlock();

	_packet = (MessagePacket*)_mailboxes[msgID].front().content;
	std::cout << "Packet2: " << _packet->getTransmittor() << std::endl;

	cvs[msgID].notify_one();

	return numBytes;
}

int MailBox::recv(uint16_t msgID, void *packet, int max){
	int numBytes = 0;
	
	// critical section
	std::unique_lock<std::mutex> lk(_mtx[msgID]);
	bool empty = _mailboxes[msgID].empty();
	//cvs[msgID].wait(lk, [empty]{return !empty;});

	// copy the message at msgID into the buffer and then pop()
	// the message from the queue
	//strcpy((char*)packet, _mailboxes[msgID].front().content);
	// packet = _mailboxes[msgID].front().content;

	// std::cout << _mailboxes[msgID].front().content -> getTransmittor() << std::endl;

	std::cout << "Packet Addr: " << packet << std::endl;
	packet = _mailboxes[msgID].front().content;
	std::cout << "Mailbox Packet Addr: " << packet << std::endl;

	MessagePacket* p = (MessagePacket*)packet;
	std::cout << "Receive packet: " << std::endl;
	std::cout << "P Addr: " << p << std::endl;
	std::cout << p-> getTransmittor() << std::endl;
	std::cout << p-> getReceiver() << std::endl;
	std::cout << p-> getSender() << std::endl;
	std::cout << p-> getDestination() << std::endl;
	
	
	_mailboxes[msgID].pop();
	
	// get the size of the packet
	numBytes = sizeof(packet);

	return numBytes;
}


bool mbox_empty(uint16_t msgID)
{
	return mailbox.empty(msgID);
}

bool mbox_avail(uint16_t msgID)
{
	return true;
}

int  mbox_send(uint16_t msgID, const void *packet, int len)
{
	return mailbox.send(msgID, packet, len);
}

int  mbox_recv(uint16_t msgID, void *packet, int max)
{
	return mailbox.recv(msgID, packet, max);
}

