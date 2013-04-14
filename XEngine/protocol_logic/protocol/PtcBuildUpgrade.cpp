#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"

#include "PtcBuildUpgrade.h"

#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"

void CBuildUpgradeProtocol::Process(UINT32 dwConnID)
{
	//printf("Recv CBuildUpgradeProtocol.\n" );

	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->UpgradeBuild(m_iUniID);
	}



	//CClientLink::Instance()->Send(dwConnID, *this);
}