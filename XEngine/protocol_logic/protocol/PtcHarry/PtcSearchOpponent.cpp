#include "../Coder_Encoder/pch.h"

#include <stdio.h>

#include "../../PlayerMgr/Player.h"
#include "../../PlayerMgr/PlayerMgr.h"
//#include "../../clientlink.h"

#include "PtcSearchOpponent.h"

#include "../../HarryMgr/HarryMgr.h"

void CSearchOpponentProtocol::Process(UINT32 dwConnID)
{
	CHarryMgr::Instance()->SearchOpponentForPlayer(dwConnID);		

	//CAckPlayerInfoProtocol ptcPlayerInfoAck;
	//ptcPlayerInfoAck.m_strName	= "";
	//ptcPlayerInfoAck.m_iRMB		= 0;

	//CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	//if(pPlayer!=NULL)
	//{
	//	ptcPlayerInfoAck.m_strName	= pPlayer->GetName();
	//	ptcPlayerInfoAck.m_iRMB		= pPlayer->GetRMB();
	//	ptcPlayerInfoAck.m_iGold	= pPlayer->GetGold();
	//	ptcPlayerInfoAck.m_iOil		= pPlayer->GetOil();
	//	ptcPlayerInfoAck.m_oBuildings	=	pPlayer->GetBuildingDataVec();

	//}
	//CClientLink::Instance()->Send(dwConnID, ptcPlayerInfoAck);
}

void CAckSearchOpponentProtocol::Process(UINT32 dwConnID)
{
}