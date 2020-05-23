#ifndef _MSG_MGR_H
#define _MSG_MGR_H
#include"map"
#include"../global.h"
class MsgMgr;
class Msg;
class Actor;
static MsgMgr* mgs_mgr_instance;
static WMutex msg_mgr_mutex = NULL;
class MsgMgr {
private:
	int m_uid = 0;
	std::map<int, Msg*>m_list;

	MsgMgr();
	~MsgMgr();
public:
	int Count = 0;
	static MsgMgr* GetSingleton() {
		if (mgs_mgr_instance == NULL) {
			mgs_mgr_instance = new MsgMgr();
		}
		return mgs_mgr_instance;
	}
	static Msg* NewMsg(void* Data, Actor* aimActor) {
		if (!msg_mgr_mutex)INIT_MUTEX(msg_mgr_mutex);
		LOCK(msg_mgr_mutex);
		Msg* msg = GetSingleton()->DoNewMsg(Data, aimActor);
		UNLOCK(msg_mgr_mutex);
		return msg;
	}
	Msg* DoNewMsg(void*, Actor*);
	Msg* GetMsg(int);
	void DelMsg(int);
	void AddMsg(Msg*);
};
#endif