#ifndef _MSG_H
#define _MSG_H
class Actor;
class Msg {
private:

public:
	Actor* AimActor;
	Actor* FromActor;
	int MsgID;
	void* MsgData;
	Msg(int, void*, Actor*);
	~Msg();
};
#endif