#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"

#include "PtcBuildResourceCollect.h"

#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"

void CBuildResourceCollectProtocol::Process(UINT32 dwConnID)
{
	//printf("Recv CBuildResourceCollectProtocol.\n" );

	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->CollectBuildResource(m_iUniID,m_iGold,m_iOil);
	}


	//CClientLink::Instance()->Send(dwConnID, *this);
}

void CAckBuildResourceCollectProtocol::Process(UINT32 dwConnID)
{
	return;
}