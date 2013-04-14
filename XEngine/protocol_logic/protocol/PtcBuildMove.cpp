#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"

#include "PtcBuildMove.h"


#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"


void CBuildMoveProtocol::Process(UINT32 dwConnID)
{
	//printf("Recv CBuildMoveProtocol.\n" );

	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->MoveBuild(m_iUniID,m_iGridX,m_iGridY);
	}


	//CClientLink::Instance()->Send(dwConnID, *this);
}