#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerMgr.h"
//#include "../clientlink.h"

#include "PtcAddResouce.h"

void CReqAddResourceProtocol::Process(UINT32 dwConnID)
{
	CAckAddResourceProtocol ptcAddResouceAck;
	ptcAddResouceAck.eResourceType	= eResourceType;
	ptcAddResouceAck.iValue		= iValue;
	ptcAddResouceAck.iResult	= -1;

	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer!=NULL)
	{
		switch(eResourceType) 
		{
		case eRMB:
			break;
		default:
			break;
		//pPlayer->
		}
	}
	//CClientLink::Instance()->Send(dwConnID, ptcPlayerInfoAck);
}

void CAckAddResourceProtocol::Process(UINT32 dwConnID)
{
}