#ifndef __PROTOCOL_LOGIC_INTERFACE_H__
#define __PROTOCOL_LOGIC_INTERFACE_H__
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include "../protocol/Coder_Encoder/coder.h"
#include "../protocol/Coder_Encoder/protocol.h"
typedef unsigned int			GUIDKEY;
using namespace std;
typedef struct SendMsg
{
	GUIDKEY							nClientID;
	vector<string>					vSend;
}stSendMsg;
class CProtocolLogicInterface
{
public:
	virtual void SetSendMsg(GUIDKEY nClientID,const CProtocol & roPtc) = 0;								///This is for the game logic to send msg;
	virtual int GetSendMsgCount(void) = 0;																///To get the send msg number;
	virtual void GetSendMsg(int nNode,GUIDKEY &nClientID,vector<string> &vSendMsg) = 0;					///To get the send msg info.It need to node number.So here will repeat and repeat again.
	virtual void ClearSendMsg(void) = 0;																///To clear the all msg buffer

	virtual void SetDisconnectClientID(GUIDKEY nClientID) = 0;											///To set disconnect ID
	virtual int GetDisconnectClientIDCount(void) = 0;													///To get disconnect ID number;
	virtual GUIDKEY GetDisconnectClientID(int nNode) = 0;												///To get disconnect ID follow the buffer index
	virtual void ClearDisconnectClientID(void) = 0;														///To clear the buffer

	virtual void OnRecv(GUIDKEY nClientID,vector<string> vRecvMsg) = 0;									///When the client connect
	virtual void OnDisconnected(GUIDKEY nClientID) = 0;
protected:
	CProtocolLogicInterface(){};
	virtual ~CProtocolLogicInterface(){};
};
CProtocolLogicInterface * CreateProtocolLogic(void);
#endif