#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"

#include "PtcBuildErase.h"

#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"

void CBuildEraseProtocol::Process(UINT32 dwConnID)
{
	//printf("Recv CBuildEraseProtocol.\n" );
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->EraseBuild(m_iUniID);
	}
	

	//CClientLink::Instance()->Send(dwConnID, *this);
}