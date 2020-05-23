#include"w_thread.h"
#include"../global.h"
#include"w_thread_mgr.h"

#if defined(__linux__)
#include<pthread.h>
#elif defined(_WIN32)
//
#include"stdio.h"
#include <windows.h>
#include <Winbase.h>
#endif

#if defined(__linux__)
static void* WThreadFunc(void* arg);
#elif defined(_WIN32)
DWORD WINAPI  WThreadFunc(void* arg);
#endif

WThread::WThread(void(*func)(LPVOID), void* arg) {
	INIT_MUTEX(this->m_mutex_send);
	INIT_MUTEX(this->m_mutex_recv);
	INIT_MUTEX(this->m_mutex_status);
	this->Func = func;
	if (this->m_arg == NULL) {
		this->m_arg = new ArgData();
	};
	this->m_arg->args = arg;
	this->m_arg->wThread = this;
	this->ID = 0;
	this->SetStatus(WThreadStatus::Status_none);
	this->Start();
}

WThread::~WThread() {
	CLOSE_MUTEX(this->m_mutex_send);
	CLOSE_MUTEX(this->m_mutex_recv);
	CLOSE_MUTEX(this->m_mutex_status);
}




WThreadStatus WThread::GetStatus() {
	WThreadStatus status;
	WaitForSingleObject(this->m_mutex_status, INFINITE);
	status = this->m_status;
	ReleaseMutex(this->m_mutex_status);
	return status;
}
void WThread::SetStatus(WThreadStatus status) {
	WaitForSingleObject(this->m_mutex_status, INFINITE);
	this->m_status = status;
	ReleaseMutex(this->m_mutex_status);
}

void WThread::SetRunArg(void(*func)(LPVOID), void* arg) {
	this->Func = func;
	this->m_arg->args = arg;
}

bool WThread::Start() {
	switch (this->GetStatus()) {
	case WThreadStatus::Status_none:
#if defined(__linux__)
		// ((pthread_create(&tidp, NULL, pthread, (void*)b)) == -1) todo
#elif defined(_WIN32)
		this->ID = (int)CreateThread(NULL, 0, WThreadFunc, this->m_arg, 0, NULL);
#endif
		break;
	case WThreadStatus::Status_running:
		return false;
		break;
	case WThreadStatus::Status_stop:
		this->SetStatus(WThreadStatus::Status_running);
		break;
	}
	return true;
}

void WThread::Stop() {
	this->SetStatus(WThreadStatus::Status_stop);

}



#if defined(__linux__)
static void* WThreadFunc(void* arg)
#elif defined(_WIN32)
DWORD WINAPI  WThreadFunc(void* arg)
#endif
{
	ArgData* argData = (ArgData*)arg;
	WThread* wThread = argData->wThread;
	while (wThread->GetStatus() != WThreadStatus::Status_exit) {
		while (wThread->GetStatus() == WThreadStatus::Status_stop)Sleep(1);
		wThread->Func(argData->args);
		wThread->Stop();
		Sleep(100);
	}
	return NULL;
}