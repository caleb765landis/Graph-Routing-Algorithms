/*
 * mailbox.cc
 * Copyright(c) 2022 Dongsoo S. Kim
 */
#include <iostream>
#include <string.h>
#include <map>
#include <queue>
#include <mutex>
#include "mbox.h"

class MailBox {
		struct item {
			int length;
			char *content;
		};

		typedef std::queue<item> mailbox_t;       // defining a queue of mailboxes to be a type for simple annotation
		std::map<uint16_t,mailbox_t> _mailboxes;  // the queue of mailboxes mapped to an unsigned integer
		std::mutex mtx;
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
	mtx.lock(); 
	for (auto x : _mailboxes) {
		while (!x.second.empty()) {
			xtem = x.second.front();
			x.second.pop(); 
			delete [] xtem.content; 
		}
	}
	mtx.unlock(); 
}

bool MailBox::empty(uint16_t msgID){
	bool isEmpty = false;

	// critical section
	mtx.lock();
	if(_mailboxes[msgID].empty() )
		isEmpty = true;
	mtx.unlock();
	
	return isEmpty;
}

int MailBox::send(uint16_t msgID, const void *packet, int len){
	item msg = {len, (char*)packet};
	int numBytes = sizeof(msg);

	// could be critical section
	//mtx.lock();
	_mailboxes[msgID].push(msg);
	//mtx.unlock();

	return numBytes;
}

int MailBox::recv(uint16_t msgID, void *packet, int max){
	int numBytes = 0;
	
	// critical section
	mtx.lock();

	// copy the message at msgID into the buffer and then pop()
	// the message from the queue
	strcpy((char*)packet, _mailboxes[msgID].front().content);
	_mailboxes[msgID].pop();
	
	// get the size of the packet
	numBytes = sizeof(packet);
	mtx.unlock();


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

