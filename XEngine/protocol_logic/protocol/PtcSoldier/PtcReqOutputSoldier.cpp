#include "../Coder_Encoder/pch.h"

#include <stdio.h>

#include "../../PlayerMgr/Player.h"
#include "../../PlayerMgr/PlayerMgr.h"
//#include "../../clientlink.h"

#include "PtcReqOutputSoldier.h"


void CReqOutputSoldierProtocol::Process(UINT32 dwConnID)
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		pPlayer->ReqOutputSoldier(m_iUniID,m_iSoldierID);
	}

}

void CAckOutputSoldierProtocol::Process(UINT32 dwConnID)
{
}
