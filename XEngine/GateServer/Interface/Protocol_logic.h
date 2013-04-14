#ifndef __PROTOCOL_LOGIC_H__
#define __PROTOCOL_LOGIC_H__

#include "../protocol/Coder_Encoder/ptcregister.h "
#include "../protocol/Coder_Encoder/singleton.h"
#include "../PlayerMgr/PlayerMgr.h"
#include "Protocol_logic_Interface.h"



class CProtocolLogic : public CProtocolLogicInterface
{
public:
	static CProtocolLogic * GetProtocolLogic(void);
	void ReleaseProtocolLogic(void);
	
	void SetSendMsg(GUIDKEY nClientID,const CProtocol & roPtc);								///This is for the game logic to send msg;
	int GetSendMsgCount(void);																///To get the send msg number;
	void GetSendMsg(int nNode,GUIDKEY &nClientID,vector<string> &vSendMsg);					///To get the send msg info.It need to node number.So here will repeat and repeat again.
	void ClearSendMsg(void);																///To clear the all msg buffer

	void SetDisconnectClientID(GUIDKEY nClientID);											///To set disconnect ID
	int GetDisconnectClientIDCount(void);													///To get disconnect ID number;
	GUIDKEY GetDisconnectClientID(int nNode);												///To get disconnect ID follow the buffer index
	void ClearDisconnectClientID(void);														///To clear the buffer
	
	void OnRecv(GUIDKEY nClientID,vector<string> vRecvMsg);									///When the client connect
	void OnDisconnected(GUIDKEY nClientID);
private:
	void CheckIsMsg(GUIDKEY nClientID,vector<string> vRecv);
	void AnalysisMsg(vector<string> vRecv,stCheckMsg *pCheckMsg);
	CProtocolLogic();
	~CProtocolLogic();
	static CProtocolLogic *						m_hHandle;									///It's handle

	vector<stSendMsg *>							m_vSendMsg;									///To save you will send the msg
	vector<GUIDKEY>								m_vDisconnectID;
	CProtocolCoder								m_oCoder;									///It's protocol handle
	map<GUIDKEY,stCheckMsg *>					m_mCheckMsg;
};
#endif