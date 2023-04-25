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

	int numBytes;
	// could be critical section
	{
		std::lock_guard<std::mutex> lock(_mtx[msgID]);

		char* msg = (char*)malloc(len + 1);
		strcpy(msg, (char*)packet);

		item pckt = {len, msg};
		numBytes = sizeof(pckt);

		// std::cout << msgID << " - MAILBOX SEND: " << msg << std::endl;
		_mailboxes[msgID].push(pckt);
	}

	cvs[msgID].notify_all();
	return numBytes;
}

int MailBox::recv(uint16_t msgID, void *packet, int max){
	int numBytes = 0;

	// critical section
	std::unique_lock<std::mutex> lk(_mtx[msgID]);
	// bool empty = _mailboxes[msgID].empty();
	// std::cv_status timedOut = cvs[msgID].wait_for(lk, std::chrono::milliseconds(200));
	cvs[msgID].wait_for(lk, std::chrono::milliseconds(1000));
	// std::cerr << msgID << " - Mailbox - Receive" << std::endl;
	// bool timedOut = cvs[msgID].wait_for(lk, std::chrono::milliseconds(200), [empty]{return !empty;});

	if(!(_mailboxes[msgID].empty())){
		strcpy((char*)packet, _mailboxes[msgID].front().content);	
		free(_mailboxes[msgID].front().content);
		_mailboxes[msgID].pop();
		numBytes = sizeof(packet);
	}

	// std::cerr << msgID << " - Mailbox - Receive - End" << std::endl;
	// }

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

