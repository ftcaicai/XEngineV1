#include "../Coder_Encoder/pch.h"

#include <stdio.h>

#include "../../PlayerMgr/Player.h"
#include "../../PlayerMgr/PlayerMgr.h"
//#include "../../clientlink.h"

#include "PtcReqCreateSoldier.h"

#include "../../HarryMgr/HarryMgr.h"

void CReqCreateSoldierProtocol::Process(UINT32 dwConnID)
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->ReqCreateSoldier(m_iUniID,m_iSoldierID);
	}
}

void CAckCreateSoldierProtocol::Process(UINT32 dwConnID)
{
}

void CAckSoldierInCreatingProtocol::Process(UINT32 dwConnID)
{
}

void CAckSoldierBeginCreateProtocol::Process(UINT32 dwConnID)
{
}

void CNotifyServerPutSoldierProtocol::Process(UINT32 dwConnID)
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->OnPutSoldier(m_iSoldierID);
	}
}