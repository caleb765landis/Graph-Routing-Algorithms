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

	// could be critical section
	_mtx[msgID].lock();;

	char* msg = (char*)malloc(len + 1);
	strcpy(msg, (char*)packet);

	item pckt = {len, msg};
	int numBytes = sizeof(pckt);

	// std::cout << msgID << " - MAILBOX SEND: " << msg << std::endl;
	_mailboxes[msgID].push(pckt);
	_mtx[msgID].unlock();

	
	
	cvs[msgID].notify_all();

	return numBytes;
}

int MailBox::recv(uint16_t msgID, void *packet, int max){
	int numBytes = 0;
	// char* buffer = (char*)malloc(max);

	// critical section
	std::unique_lock<std::mutex> lk(_mtx[msgID]);
	bool empty = _mailboxes[msgID].empty();


	cvs[msgID].wait_for(lk, std::chrono::milliseconds(100) ,[empty]{return !empty;});

	// copy the message at msgID into the buffer and then pop()
	// the message from the queue
	// std::cout << msgID << " - MAILBOX RECV - IN - " << _mailboxes[msgID].front().content << std::endl;

	strcpy((char*)packet, _mailboxes[msgID].front().content);
	// std::cout << msgID << " - MAILBOX RECV - OUT - " << (char*)packet << std::endl;
	
	free(_mailboxes[msgID].front().content);
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

