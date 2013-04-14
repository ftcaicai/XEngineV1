#include "pch.h"
#include "ptcregister.h"

#include "../../protocol/testptc.h"

void CProtocolRegister::Regiter()
{
	 CProtocol::Register(new CTestProtocol());
}