#include"msg.h"
#include"msg_mgr.h"
#include"../global.h"
MsgMgr::MsgMgr() {

}

MsgMgr::~MsgMgr() {
	std::map<int, Msg*>::iterator iter = this->m_list.begin();
	while (iter != this->m_list.end()) {
		SafeDelete(iter->second);
		iter++;
	}
	this->m_list.clear();
	//CLOSE_MUTEX(msg_mgr_mutex);
}

// ·ÏÆú
Msg* MsgMgr::DoNewMsg(void* data, Actor* aimActor) {
	int MsgId = this->m_uid++;
	this->Count++;
	//printf("add %d\n", MsgId);
	return new Msg(MsgId, data, aimActor);
}
Msg* MsgMgr::GetMsg(int msgId) {
	std::map<int, Msg*>::iterator iter;
	if ((iter = this->m_list.find(msgId)) != this->m_list.end()) {
		return iter->second;
	}
	return NULL;
}
void MsgMgr::DelMsg(int msgId) {
	LOCK(msg_mgr_mutex);
	//printf("del %d\n", msgId);
	this->Count--;
	UNLOCK(msg_mgr_mutex);
	//this->m_list.erase(msgId);
}
void MsgMgr::AddMsg(Msg* msg) {
	this->m_list.insert(std::map<int, Msg*>::value_type(msg->MsgID, msg));
}