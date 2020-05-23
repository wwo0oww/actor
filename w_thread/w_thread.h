#ifndef _W_THREAD_H
#define _W_THREAD_H
#include"../global.h"

enum class WThreadStatus :int {
	Status_none = 0,
	Status_running = 1,
	Status_stop = 2,
	Status_exit = 3,
};
static WMutex t_mutex;
class WThread;
typedef struct {
	void* args;
	WThread* wThread;
}ArgData;
class WThread {
private:
	WMutex m_mutex_send;
	WMutex m_mutex_recv;
	WMutex m_mutex_status;

	WThreadStatus m_status;
	ArgData* m_arg;
public:
	

	int ID;
	void(*Func)(LPVOID);
	WThread(void(*)(LPVOID), void*);
	~WThread();
	static void Init() { INIT_MUTEX(t_mutex); }
	bool Start();
	void Stop();
	WThreadStatus GetStatus();
	void SetStatus(WThreadStatus);
	void SetRunArg(void(*func)(LPVOID), void* arg);
	void AddMsg();
	void SendMsg();

};
#endif