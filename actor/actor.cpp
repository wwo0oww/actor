#include"actor.h"
#include"../w_thread/w_thread_mgr.h"


void SendLoop(LPVOID arg) {
	Actor* actor = (Actor*)arg;
	while (actor->Running)
	{
		Msg* msg;
		if ((msg = actor->GetOneSendMsg()) != NULL) {
			//std::cout << "send msg to " << msg->AimActor->Name << ":" << std::string((char*)msg->MsgData) << std::endl;
			msg->AimActor->AddOneMsg(msg);
		}
		Sleep(1);
	}
}

void Update(LPVOID arg) {
	Actor* actor = (Actor*)arg;
	while (actor->Running)
	{
		Msg* msg;
		if ((msg = actor->GetOneRecvMsg()) != NULL) {
			//std::cout << "recv msg from " << msg->FromActor->Name << ":" << std::string((char*)msg->MsgData) << std::endl;
			SafeDelete(msg);
		}
		Sleep(1);
	}
}

MsgNode* add_msg_node(MsgNode* node) {
	MsgNode* newNode = new MsgNode();
	memset(newNode, NULL, sizeof(MsgNode));
	node->next = newNode;
	//printf("增加节点%d\n", node->index + 1);
#ifdef DEBUG
	newNode->index = node->index + 1;
#endif // DEBUG
	return newNode;
}

Actor::Actor(std::string name) {
	this->Name = name;
	this->m_recv = new MsgList();

	memset(this->m_recv, NULL, sizeof(MsgList));

	this->m_recv->head = new MsgNode();

	memset(this->m_recv->head, NULL, sizeof(MsgNode));

	MsgNode* tmp = this->m_recv->head;

	for (int i = 0; i < DATA_LENGTH - 1; i++)
		tmp = add_msg_node(tmp);

	this->m_recv->readNode = this->m_recv->head;

	this->m_recv->writeNode = this->m_recv->head;

	INIT_MUTEX(this->m_recv->m_mutex);





	this->m_send = new MsgList();

	memset(this->m_send, NULL, sizeof(MsgList));

	this->m_send->head = new MsgNode();

	memset(this->m_send->head, NULL, sizeof(MsgNode));

	tmp = this->m_send->head;

	for (int i = 0; i < DATA_LENGTH - 1; i++)
		tmp = add_msg_node(tmp);

	this->m_send->readNode = this->m_send->head;

	this->m_send->writeNode = this->m_send->head;

	INIT_MUTEX(this->m_send->m_mutex);

	this->Running = true;

	WThreadMgr::NewThread(SendLoop, this);
	WThreadMgr::NewThread(Update, this);
}

Msg* Actor::GetOneRecvMsg() {
	MsgNode* msgNode = this->m_recv->readNode;
	if (msgNode->msg != NULL) {
		Msg* msg = msgNode->msg;
		msgNode->msg = NULL;
		if (this->m_recv->readNode->next == NULL) {
			this->m_recv->readNode = this->m_recv->head;
		}
		else {
			this->m_recv->readNode = this->m_recv->readNode->next; //最后才修改readNode，防止其他异步写线程出问题
		}
		return msg;
	}
	return NULL;
}

void Actor::AddOneMsg(Msg* msg) {
	LOCK(this->m_recv->m_mutex);
	this->m_recv->writeNode->msg = msg;
	if (this->m_recv->writeNode->next == this->m_recv->readNode || (this->m_recv->writeNode->next == NULL && this->m_recv->readNode == this->m_recv->head)) {
		MsgNode* tmp = m_recv->writeNode; // 追上read了，申请新的长度
		MsgNode* next = m_send->writeNode->next;
		//std::cout << "申请新的长度:" << this->Name << std::endl;
		for (int i = 0; i < DATA_LENGTH; i++)
			tmp = add_msg_node(tmp);
		tmp->next = next; // 防止readNode 已经变化了，直接用this->m_recv->writeNode->next
	}
	if (this->m_recv->writeNode->next == NULL) {
		this->m_recv->writeNode = this->m_recv->head;
	}
	else {
		this->m_recv->writeNode = this->m_recv->writeNode->next;
	}
	UNLOCK(this->m_recv->m_mutex);
}

Msg* Actor::GetOneSendMsg() {
	MsgNode* msgNode = this->m_send->readNode;
	if (msgNode->msg != NULL) {
		Msg* msg = msgNode->msg;
		msgNode->msg = NULL;
		if (this->m_send->readNode->next == NULL) {
			this->m_send->readNode = this->m_send->head;
		}
		else {
			this->m_send->readNode = this->m_send->readNode->next; //最后才修改readNode，防止其他异步写线程出问题
		}
		return msg;
	}
	return NULL;
}

void Actor::SendOneMsg(Msg* msg) {
	msg->FromActor = this;
	this->m_send->writeNode->msg = msg;
	//printf("发送消息【%d】：%d \n", this->m_send->writeNode->index, msg->MsgID);
	if (this->m_send->writeNode->next == this->m_send->readNode || (this->m_send->writeNode->next == NULL && this->m_send->readNode == this->m_send->head)) {
		MsgNode* tmp = m_send->writeNode; // 追上read了，申请新的长度
		MsgNode* next = m_send->writeNode->next;
		for (int i = 0; i < DATA_LENGTH; i++)
			tmp = add_msg_node(tmp);
		tmp->next = next; // 防止readNode 已经变化了，直接用this->m_send->writeNode->next
	}
	if (this->m_send->writeNode->next == NULL) {
		this->m_send->writeNode = this->m_send->head;
	}
	else {
		this->m_send->writeNode = this->m_send->writeNode->next;
	}
}


Actor::~Actor() {
	CLOSE_MUTEX(this->m_recv->m_mutex);
	CLOSE_MUTEX(this->m_send->m_mutex);
	MsgNode* tmp = this->m_recv->head;
	while (tmp) {
		MsgNode* tmp1 = tmp->next;
		delete(tmp);
		tmp = tmp1;
	}
	tmp = this->m_send->head;
	while (tmp) {
		MsgNode* tmp1 = tmp->next;
		delete(tmp);
		tmp = tmp1;
	}
	SafeDelete(this->m_recv);
	SafeDelete(this->m_send);

}
//Msg* Actor::GetOneMsg() {
//
//}