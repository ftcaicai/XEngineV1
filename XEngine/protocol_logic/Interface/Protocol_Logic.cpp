#include "Protocol_logic.h"
CProtocolLogic * CProtocolLogic::m_hHandle = NULL;
CProtocolLogic::CProtocolLogic()
{

}
CProtocolLogic::~CProtocolLogic()
{
	if(m_hHandle)
	{
		delete m_hHandle;
		m_hHandle = NULL;
	}
	else
		m_hHandle = NULL;
}
CProtocolLogic * CProtocolLogic::GetProtocolLogic(void)
{
	if(m_hHandle == NULL)
	{
		m_hHandle = new CProtocolLogic;
		if(m_hHandle)
			return m_hHandle;
		else
			return NULL;
	}
	else
		return m_hHandle;
}
void CProtocolLogic::ReleaseProtocolLogic(void)
{
	if(m_hHandle)
	{
		delete m_hHandle;
		m_hHandle = NULL;
	}
	else
		m_hHandle = NULL;
}
void CProtocolLogic::SetSendMsg(GUIDKEY nClientID,const CProtocol & roPtc)
{
	string					sSend;
	stSendMsg *				pSend = new stSendMsg;
	sSend.clear();
	sSend.append(m_oCoder.GetData(),m_oCoder.GetLenth());
	pSend->nClientID = nClientID;
	pSend->vSend.push_back(sSend);

}
int CProtocolLogic::GetSendMsgCount(void)
{
	return (int)m_vSendMsg.size();
}
void CProtocolLogic::GetSendMsg(int nNode,GUIDKEY &nClientID,vector<string> &vSendMsg)
{
	if(nNode >= 0 && nNode < (int)m_vSendMsg.size())
	{
		nClientID = m_vSendMsg[nNode]->nClientID;
		vSendMsg = m_vSendMsg[nNode]->vSend;
	}
}
void CProtocolLogic::ClearSendMsg(void)
{
	for(int nCount1 = 0; nCount1 < (int)m_vSendMsg.size(); ++nCount1)
	{
		for(int nCount2 = 0; nCount2 < (int)m_vSendMsg[nCount1]->vSend.size(); ++nCount2)
		{
			m_vSendMsg[nCount1]->vSend[nCount2].clear();
		}
		m_vSendMsg[nCount1]->vSend.clear();
		delete m_vSendMsg[nCount1];
	}
	m_vSendMsg.clear();
}
void CProtocolLogic::OnRecv(GUIDKEY nClientID,vector<string> vRecvMsg)
{
	string						sRecvMsg;
	for(int nCount = 0; nCount < (int)vRecvMsg.size(); ++nCount)
	{
		sRecvMsg = vRecvMsg[nCount];
		if((int)sRecvMsg.size() < 8)
		{
			//LOG_CRI
			printf(" :: CNetProcessor::OnRecv Error dwLen < 8 , dwLen = %u",(int)sRecvMsg.size());
			return;
		}

		UINT32 dwType = 0;;
		m_oCoder.GetProtocolType(sRecvMsg.c_str(), (int)sRecvMsg.size(), dwType);

		CProtocol* poProtocol = CProtocol::GetProtocol(dwType);
		if(poProtocol == NULL)
		{
			//LOG_CRI
			printf(" :: CNetProcessor::OnRecv Cannot Find Protocol Type = %d\n",dwType);
			return;
		}

		printf(" :: CNetProcessor::OnRecv Protocol Type = %d\n",dwType);

		m_oCoder.Decode(sRecvMsg.c_str() + 8, (int)sRecvMsg.size() - 8, *poProtocol);
		poProtocol->Process(nClientID);
	}
}
void CProtocolLogic::SetDisconnectClientID(GUIDKEY nClientID)
{
	m_vDisconnectID.push_back(nClientID);
}
int CProtocolLogic::GetDisconnectClientIDCount(void)
{
	return (int)m_vDisconnectID.size();
}
GUIDKEY CProtocolLogic::GetDisconnectClientID(int nNode)
{
	if(nNode >= 0 && nNode < (int)m_vDisconnectID.size())
	{
		return m_vDisconnectID[nNode];
	}
	else
		return 0;
	
}
void CProtocolLogic::ClearDisconnectClientID(void)
{
	m_vDisconnectID.clear();
}
void CProtocolLogic::OnDisconnected(GUIDKEY nClientID)
{
	CPlayerMgr::Instance()->ReleasePlayer(nClientID);
}
CProtocolLogicInterface * CreateProtocolLogic(void)
{
	CProtocolLogicInterface * hHandle;
	hHandle = static_cast<CProtocolLogicInterface *>(CProtocolLogic::GetProtocolLogic());
	if(hHandle != NULL)
	{
		return hHandle;
	}
	else
		return NULL;
}