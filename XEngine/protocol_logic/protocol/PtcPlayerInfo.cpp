#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include <atltime.h>

#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"
//#include "../clientlink.h"
#include "../Interface/Protocol_logic.h"

#include "PtcPlayerInfo.h"

void CReqPlayerInfoProtocol::Process(UINT32 dwConnID)
{
	CAckPlayerInfoProtocol ptcPlayerInfoAck;
	ptcPlayerInfoAck.m_strName	= "";
	ptcPlayerInfoAck.m_iRMB		= 0;

	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		CTime tm;
		tm=CTime::GetCurrentTime();
		__time64_t now = tm.GetTime();

		ptcPlayerInfoAck.m_TimeNow  = now;
		ptcPlayerInfoAck.m_strName	= pPlayer->GetName();
		ptcPlayerInfoAck.m_iRMB		= pPlayer->GetRMB();
		ptcPlayerInfoAck.m_iGold	= pPlayer->GetGold();
		ptcPlayerInfoAck.m_iOil		= pPlayer->GetOil();
		ptcPlayerInfoAck.m_iPopulation = pPlayer->GetPopulation();
		ptcPlayerInfoAck.m_oBuildings  = pPlayer->GetBuildingDataVec();
		pPlayer->GetBuilderDataToClient(ptcPlayerInfoAck.m_oBuilderDataToClient);
		pPlayer->GetLabData(ptcPlayerInfoAck.m_labdata);
		pPlayer->GetSoldierData(ptcPlayerInfoAck.m_SoldierData);
		pPlayer->GetCampDataToClient(ptcPlayerInfoAck.m_mapUniID2CampDataToClient);

	}
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(dwConnID,ptcPlayerInfoAck);
	//CClientLink::Instance()->Send(dwConnID, ptcPlayerInfoAck);
}

void CAckPlayerInfoProtocol::Process(UINT32 dwConnID)
{
}