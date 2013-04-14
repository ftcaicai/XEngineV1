#include "../Coder_Encoder/pch.h"

#include <stdio.h>

#include "../../PlayerMgr/Player.h"
#include "../../PlayerMgr/PlayerMgr.h"
//#include "../../clientlink.h"

#include "PtcReqUpgradeSoldier.h"


void CReqUpgradeSoldierProtocol::Process(UINT32 dwConnID)
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->ReqUpgradeSoldier(m_iSoldierID);
	}
}

void CAckUpgradeSoldierProtocol::Process(UINT32 dwConnID)
{
}

void CAckUpgradeSoldierSuccProtocol::Process(UINT32 dwConnID)
{
}