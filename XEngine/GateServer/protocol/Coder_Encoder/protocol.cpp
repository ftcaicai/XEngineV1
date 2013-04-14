#include <stdio.h>
#include "define.h"
#include "protocol.h"

CProtocol::CProtocolMap CProtocol::sm_oPtcMap;


bool CProtocol::Register(CProtocol* poProtocol)
{
	if(poProtocol == NULL)
	{
		return false;
	}

	std::pair<CProtocolMap::iterator, bool> pr = sm_oPtcMap.insert(std::make_pair(poProtocol->m_dwType, poProtocol));
	if(!pr.second)
	{
		return false;
	}

	return true;
}

void CProtocol::UnRegister()
{
	for(CProtocolMap::iterator it = sm_oPtcMap.begin(); it != sm_oPtcMap.end(); ++it)
	{
		CProtocol* poProtocol = it->second; 
		delete poProtocol;
	}

	sm_oPtcMap.clear();
}

CProtocol* CProtocol::GetProtocol(UINT32 dwType)
{
	CProtocolMap::iterator it = sm_oPtcMap.find(dwType);
	return it == sm_oPtcMap.end() ? NULL : it->second;
}