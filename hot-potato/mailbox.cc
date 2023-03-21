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
  typedef std::queue<item> mailbox_t; 
  std::map<uint16_t,mailbox_t> _mailboxes;
  std::mutex mtx; 
public:
  MailBox() {};
  ~MailBox();
  bool empty(uint16_t mid); 
  int  send(uint16_t mid, const void *packet, int len);
  int  recv(uint16_t mid, void *packet, int max);
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


bool mbox_empty(uint16_t mid)
{
  return mailbox.empty(mid);
}

bool mbox_avail(uint16_t mid)
{
  return true; 
}


int  mbox_send(uint16_t mid, const void *packet, int len)
{
  return mailbox.send(mid, packet, len);
}


int  mbox_recv(uint16_t mid, void *packet, int max)
{
  return mailbox.recv(mid, packet, max);
}

