#ifndef _ACTOR_H
#define _ACTOR_H
#include"../global.h"
#include"../msg/msg.h"
#include <iostream>
#include<string>

#define DATA_LENGTH 10

typedef struct MsgNode {
	Msg* msg;
	MsgNode* next;
#ifdef DEBUG
	int index;
#endif
}*PMsgNode;

typedef struct MsgList {
	MsgNode* head;
	MsgNode* readNode;
	MsgNode* writeNode;
	WMutex m_mutex;
}*PMsgList;

class Actor {
private:

	MsgList* m_recv;
	MsgList* m_send;


public:
	bool Running;
	std::string Name;
	Actor(std::string);
	~Actor();
	Msg* GetOneRecvMsg();
	Msg* GetOneSendMsg();
	void AddOneMsg(Msg*);
	void SendOneMsg(Msg*);
};
#endif
