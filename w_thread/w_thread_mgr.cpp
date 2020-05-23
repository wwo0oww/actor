#include"w_thread.h"
#include"w_thread_mgr.h"
#include"../global.h"
WThreadMgr::WThreadMgr() {
}
WThreadMgr::~WThreadMgr() {
	std::map<int, WThread*>::iterator iter = this->m_list.begin();
	while (iter != this->m_list.end()) {
		SafeDelete(iter->second);
		iter++;
	}
	this->m_list.clear();
	CLOSE_MUTEX(wthread_mgr_mutex);
}

int WThreadMgr::Num() { 
	return m_list.size(); 
}

WThread* WThreadMgr::DoNewThread(void (*func)(LPVOID), void* arg)
{
	WThread* thread;
	if ((thread = this->get_able_thread()) != NULL) {
		thread->SetRunArg(func, arg);
		thread->Start();
		return thread;
	}
	thread = new WThread(func, arg);
	this->m_list.insert(std::pair<int, WThread*>(thread->ID, thread));
	return thread;
}

// todo 优化：回收列表另外用个列表
WThread* WThreadMgr::get_able_thread() {
	if (this->m_list.size() == 0)return NULL;
	std::map<int, WThread*>::iterator iter = this->m_list.begin();
	while (iter != this->m_list.end()) {
		if (iter->second->GetStatus() == WThreadStatus::Status_stop) {
			return iter->second;
		}
		iter++;
	}
	return NULL;
}