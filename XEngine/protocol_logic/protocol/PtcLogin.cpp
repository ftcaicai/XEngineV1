#include "../protocol/Coder_Encoder/pch.h"

#include <stdio.h>

#include "../MySQL/CMysql.h"

//#include "../clientlink.h"
#include "../Interface/Protocol_logic.h"

#include "PtcLogin.h"

void CLoginProtocol::Process(UINT32 dwConnID)
{
	printf("Recv CLoginProtocol. \n" );

	//

	if (CMysql::Instance()->LoginCheck(dwConnID , m_strPassWord,m_strName) == true)
	{
		printf("LoginCheck success name = %s",m_strName.c_str());
	
		CLoginACKProtocol ptcLoginAck;
		ptcLoginAck.m_iRetNum = 0;

		CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
		pProtocol->SetSendMsg(dwConnID,ptcLoginAck);
		//CClientLink::Instance()->Send(dwConnID, ptcLoginAck);

		return;
	}
	else
	{
		printf("LoginCheck failed name = %s",m_strName.c_str());	
	}

	
	//CClientLink::Instance()->Send(dwConnID, *this);
}

void CLoginACKProtocol::Process(UINT32 dwConnID)
{
}