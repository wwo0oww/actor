#include "stdio.h"
#if defined(__linux__)
# include <pthread.h>
// Linux系统
#elif defined(_WIN32)
// Windows系统
#include <windows.h>
#include<time.h>
#include<map>
#include"w_thread/w_thread_mgr.h"
#include"w_thread/w_thread.h"
#include"actor/actor.h"
#include"msg/msg_mgr.h"
#include"msg/msg.h"
#endif
HANDLE g_Mutex;
char* shi[10] = { "寥落古行宫","宫花寂寞红","白头宫女在","闲坐说玄宗","故国三千里","深宫二十年","一声何满子","双泪落君前","绿蚁新醅酒","红泥小火炉" };

class Node {
	int s[10];
public:
	int x[10000];
	void test() {
		for (int i = 0; i < 10; i++) {
			this->s[i] = 0;
		};
		for (int i = 0; i < 10; i++) {
			this->s[i] = 20;
		};
	}
	void test1() {
		int s = 0;
		for (int i = 0; i < 10; i++) {
			s += this->s[i];
		};
	}
	~Node() {
		printf("node \n");
	}
};
class Node1 :public  Node {
public:
	~Node1() {
		printf("node 1\n");
	}
	void test2(LPVOID) {};
};

void   ThreadFunc2(LPVOID p) {
	Sleep(100000);
	printf("线程退出%d\n", (int)p);
}

Node* b = new Node();

DWORD WINAPI  ThreadFunc(LPVOID p) {



	while (true) {
		for (int i = 0; i < 100; i++) {
			WThread* thread = WThreadMgr::NewThread(ThreadFunc2, (LPVOID)i);
			Sleep(10);
			printf("total:%d\n", WThreadMgr::GetSignleton()->Num());
			//CreateThread(NULL, 0, ThreadFunc1, (LPVOID)i, 0, NULL); // 创建线程
		}
	}
}



DWORD WINAPI   ThreadFunc1(LPVOID p) {
	int t1, t2;
	while (true) {
		int s = 0;
		t1 = GetTickCount();
		b->test1();
		t2 = GetTickCount();
		printf("%d\n", int(p));
		Sleep(1000000);
	}
}

DWORD WINAPI   ThreadFuncActor(LPVOID p) {
	Actor* actor[10];
	for (int i = 0; i < 10; i++) {
		char buf[15];
		sprintf_s(buf, "test_%d", (int)p * 1000 + i);
		actor[i] = new Actor(buf);
	}
	while (true) {
		for (int j = 0; j < 10; j++) {
			for (int w = 0; w < 1000; w++) {
				srand((int)time(0));
				int j = rand() % 10;
				int i = rand() % 10;
				if (i != j) {
					actor[i]->SendOneMsg(MsgMgr::NewMsg((void*)(shi[i]), actor[j]));
				}
			}
			Sleep(1000);
		}
		while (true)
		{

		}
	}
}


int main() {

	for (int i = 0; i < 5; i++) {
		CreateThread(NULL, 0, ThreadFuncActor, (LPVOID)i, 0, NULL);
	}
	/*Actor* actor1 = new Actor("test1");
	Actor* actor2 = new Actor("test2");
	for (int i = 0; i < 2000; i++) {
		actor2->SendOneMsg(MsgMgr::NewMsg((void*)(shi[i%10]), actor1));
	}*/

	while (true) {
		printf("total msg:%d\n", MsgMgr::GetSingleton()->Count);
		Sleep(3000);
	}
}
int main1() {
	Node* node = new Node();
	int t1, t2;
	t1 = GetTickCount();
	for (int i = 0; i < 1000000; i++) {
		//node = new Node();
		node->x[0] = 10;
		//delete node;
	}
	t2 = GetTickCount();
	printf("%d\n", int(t2 - t1));
	Sleep(10000);
	WThread::Init();
	g_Mutex = CreateMutex(NULL, false, NULL);
	printf("this is a test\n");
	for (int i = 0; i < 10000; i++) {
		CreateThread(NULL, 0, ThreadFunc1, (LPVOID)i, 0, NULL); // 创建线程
	}
	for (int i = 0; i < 100; i++) {
		//WThread* thread = WThreadMgr::NewThread(ThreadFunc2, (LPVOID)i);
		Sleep(1);
		//printf("total:%d\n",WThreadMgr::GetSignleton()->Num());
		//CreateThread(NULL, 0, ThreadFunc1, (LPVOID)i, 0, NULL); // 创建线程
	}
	Sleep(1000000);
	ReleaseMutex(g_Mutex);
	CloseHandle(g_Mutex);
	return 0;
}


