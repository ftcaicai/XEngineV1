#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"
#include "../Interface/Protocol_logic.h"

#include "PtcRegisterPlayer.h"

void CRegisterPlayerProtocol::Process(UINT32 dwConnID)
{
	printf("Recv CRegisterProtocol.\n" );

	CRegisterACKProtocol			ptcRegisterAck;
	if ( CMysql::Instance()->RegisterPlayer(m_strPassWord,m_strName) == true )
	{
		// register success auto login
		// not auto login first let is Send
		
		ptcRegisterAck.m_iRetNum = 0;
		printf("RegisterPlayer success name = %s",m_strName.c_str());
	}
	else
	{
		ptcRegisterAck.m_iRetNum = -1;
		printf("RegisterPlayer failed name = %s",m_strName.c_str());	
	}
	
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(dwConnID,ptcRegisterAck);
	//CClientLink::Instance()->Send(dwConnID, *this);
}
void CRegisterACKProtocol::Process(UINT32 dwConnID)
{
}