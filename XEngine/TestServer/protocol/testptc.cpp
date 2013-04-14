#include "../protocol/Coder_Encoder/pch.h"
#include "testptc.h"
#include <stdio.h>

#include "../Interface/Protocol_logic.h"
//#include "../clientlink.h"

void CTestProtocol::Process(UINT32 dwConnID)
{
	printf("Recv CTestProtocol. %d\n", m_nID);

	m_nID++;
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(dwConnID, *this);
}