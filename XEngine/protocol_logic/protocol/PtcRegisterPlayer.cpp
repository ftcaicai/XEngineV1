#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"

#include "PtcRegisterPlayer.h"

void CRegisterPlayerProtocol::Process(UINT32 dwConnID)
{
	printf("Recv CRegisterProtocol.\n" );

	if ( CMysql::Instance()->RegisterPlayer(m_strPassWord,m_strName) == true )
	{
		// register success auto login
		// not auto login first let is Send
		printf("RegisterPlayer success name = %s",m_strName.c_str());
		return;
	}
	else
	{
		printf("RegisterPlayer failed name = %s",m_strName.c_str());	
	}

	//CClientLink::Instance()->Send(dwConnID, *this);
}