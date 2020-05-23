#ifndef _W_THREAD_MGR_H
#define _W_THREAD_MGR_H
#include"map"
#include"../global.h"

class WThread;
class WThreadMgr;
static WThreadMgr* wthread_mgr_instance = NULL;
static WMutex wthread_mgr_mutex = NULL;
class WThreadMgr {
private:
	std::map<int, WThread*>m_list;
public:
	WThreadMgr();
	~WThreadMgr();
	int Num();
	static WThreadMgr* GetSignleton() {
		if (wthread_mgr_instance == NULL) {
			wthread_mgr_instance = new WThreadMgr();
		}
		return wthread_mgr_instance;
	}
	static WThread* NewThread(void(*func)(LPVOID), void* arg) {
		if (wthread_mgr_mutex == NULL) {
			INIT_MUTEX(wthread_mgr_mutex);
		}
		LOCK(wthread_mgr_mutex);
		GetSignleton();
		WThread* wThread = wthread_mgr_instance->DoNewThread(func, arg);
		UNLOCK(wthread_mgr_mutex);
		return wThread;
	}
	WThread* get_able_thread();
	WThread* DoNewThread(void(*)(LPVOID), void*);
};
#endif