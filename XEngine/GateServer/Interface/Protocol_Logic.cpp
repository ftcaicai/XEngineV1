#include "Protocol_logic.h"
CProtocolLogic * CProtocolLogic::m_hHandle = NULL;
CProtocolLogic::CProtocolLogic()
{
	CProtocolRegister::Regiter();
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
	m_oCoder.Encode(roPtc);
	sSend.clear();
	sSend.append(m_oCoder.GetData(),m_oCoder.GetLenth());
	pSend->nClientID = nClientID;
	pSend->vSend.push_back(sSend);
	m_vSendMsg.push_back(pSend);

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
	vector<string>				vTempMsg;
	CheckIsMsg(nClientID,vRecvMsg);
	map<GUIDKEY,stCheckMsg *>::iterator it = m_mCheckMsg.find(nClientID);
	if(it != m_mCheckMsg.end())
	{
		vTempMsg.clear();
		if(it->second->bIsWaitNext == false)
		{
			vTempMsg = it->second->vCheck;
			it->second->vCheck.clear();
		}
		else
		{
			for(int nCount = 0; nCount < (int)it->second->vCheck.size() - 1; ++nCount)
			{
				vTempMsg.push_back(it->second->vCheck[nCount].c_str());
				it->second->vCheck.erase(it->second->vCheck.begin() + nCount);
			}
		}
	}
	for(int nCount = 0; nCount < (int)vTempMsg.size(); ++nCount)
	{
		sRecvMsg = vTempMsg[nCount];
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
void CProtocolLogic::CheckIsMsg(GUIDKEY nClientID,vector<string> vRecv)
{
	map<GUIDKEY,stCheckMsg *>::iterator it = m_mCheckMsg.find(nClientID);
	if(it == m_mCheckMsg.end())
	{
		stCheckMsg * pCheckMsg = new stCheckMsg;
		if(pCheckMsg != NULL)
		{
			pCheckMsg->nClientID = nClientID;
			pCheckMsg->vCheck.clear();
			pCheckMsg->bIsWaitNext = false;
			m_mCheckMsg.insert(make_pair(nClientID,pCheckMsg));
			AnalysisMsg(vRecv,pCheckMsg);
		}
	}
	else
	{
		AnalysisMsg(vRecv,it->second);
	}
}

void CProtocolLogic::AnalysisMsg(vector<string> vRecv,stCheckMsg *pCheckMsg)
{
	string						sTempMsg;
	string						sMsgSize;
	int							nMsgSize;
	int							nBeginPos;
	int							nEndPos;
	string						sMsg;
	for(int nCount = 0; nCount < (int)vRecv.size(); ++nCount)
	{
		if(pCheckMsg->bIsWaitNext == false)
		{
			sTempMsg = vRecv[nCount];
		}
		else
		{
			sTempMsg = pCheckMsg->vCheck[(int)pCheckMsg->vCheck.size() - 1];
			sTempMsg.append(vRecv[nCount].c_str(),(int)vRecv[nCount].size());
			pCheckMsg->bIsWaitNext = false;
		}
		
		nBeginPos = 0;
		while((int)sTempMsg.size() > nBeginPos)
		{
			if((int)sTempMsg.size() >= nBeginPos + 4)
			{
				sMsgSize = sTempMsg.substr(nBeginPos,4);
				nMsgSize = (*(int*)sMsgSize.c_str());
				nEndPos = nBeginPos + nMsgSize + 4;
				if(nEndPos <= (int)sTempMsg.size())
				{
					sMsg = sTempMsg.substr(nBeginPos,nEndPos);
					pCheckMsg->vCheck.push_back(sMsg);
					pCheckMsg->bIsWaitNext = false;
				}
				else
				{
					sMsg = sTempMsg.substr(nBeginPos,(int)sTempMsg.size() - nBeginPos);
					pCheckMsg->vCheck.push_back(sMsg);
					pCheckMsg->bIsWaitNext = true;
				}
				nBeginPos = nEndPos;
				
			}
		}
	}
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