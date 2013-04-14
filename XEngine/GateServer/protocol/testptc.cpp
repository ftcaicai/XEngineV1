#include "../protocol/Coder_Encoder/pch.h"
#include "testptc.h"
#include <stdio.h>


//#include "../clientlink.h"
#include "../Interface/Protocol_logic.h"

void CTestProtocol::Process(UINT32 dwConnID)
{
	printf("Recv CTestProtocol. %d\n", m_nID);
	m_strText;
	m_oData;

	if(m_nID>1352)
	{
		printf("Hello\n");
	}

	m_nID++;
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(dwConnID,*this);
	//CClientLink::Instance()->Send(dwConnID, *this);
}