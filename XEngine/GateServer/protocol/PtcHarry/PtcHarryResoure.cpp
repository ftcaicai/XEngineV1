#include "../Coder_Encoder/pch.h"

#include <stdio.h>

#include "../../MySQL/CMysql.h"

//#include "../../clientlink.h"

#include "PtcHarryResoure.h"

#include "../../PlayerMgr/Player.h"
#include "../../PlayerMgr/PlayerMgr.h"

void CHarryResourceProtocol::Process(UINT32 dwConnID)
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->HarryResource(m_iGold,m_iOil);
	}
}

void CAckHarryResourceProtocol::Process(UINT32 dwConnID)
{
	return;
}