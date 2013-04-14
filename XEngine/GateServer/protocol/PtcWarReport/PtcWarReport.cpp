#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"
#include "../Interface/Protocol_logic.h"

#include "PtcWarReport.h"
#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"

void CPlayerWarReportProtocol::Process(UINT32 dwConnID)
{
	CPlayerWarReportACKProtocol				ptcPlayerWarReport;
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->GetWarReportData(ptcPlayerWarReport.m_vecData);
	}
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(dwConnID,ptcPlayerWarReport);
}
void CPlayerWarReportACKProtocol::Process(UINT32 dwConnID)
{

}

void CPlayerWarReportUpdateProtocol::Process(UINT32 dwConnID)
{
	vector<CPlayerWarReportData>			vPlayerWarReport;
	vPlayerWarReport.clear();
	vPlayerWarReport.push_back(data);
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->UpdateWarReportData(vPlayerWarReport);
	}
}
void CPlayerWarReportSetTopProtocol::Process(UINT32 dwConnID)
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->SetWarReportTop(iIndex,iIsTop);
	}
}
void CPlayerWarReportDetailProtocol::Process(UINT32 dwConnID)
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->SetStorageDataVec(m_vecData,m_iLostBuildingPercent,m_iAccountID);
	}
}