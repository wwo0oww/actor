#include"msg.h"
#include"msg_mgr.h"
Msg::Msg(int msgId, void* data, Actor*AimActor) {
	this->MsgData = data;
	this->MsgID = msgId;
	this->AimActor = AimActor;
}
Msg::~Msg() {
	MsgMgr::GetSingleton()->DelMsg(this->MsgID);
}