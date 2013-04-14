#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"

#include "PtcBuildAdd.h"

#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"

void CBuildAddProtocol::Process(UINT32 dwConnID)
{
	//printf("Recv CBuildAddProtocol.\n" );

	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->AddBuild(m_iUniID,m_iID,m_iLevel,m_iGridX,m_iGridY);
	}
	//CClientLink::Instance()->Send(dwConnID, *this);
}