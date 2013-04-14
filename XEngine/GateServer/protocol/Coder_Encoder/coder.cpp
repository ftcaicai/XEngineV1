#include "pch.h"
#include "coder.h"
#include "protocol.h"


CProtocolCoder::CProtocolCoder()
{ 
}

CProtocolCoder::~CProtocolCoder()
{ 
}

bool CProtocolCoder::Encode(const CProtocol& roPtc)
{
	m_oStream.Clear();

	UINT32 dwLen = 0; 
	m_oStream << dwLen;
	m_oStream << roPtc.m_dwType;
	m_oStream << roPtc;

	dwLen = (UINT32)m_oStream.Size() - 4;
	m_oStream.Set(0, &dwLen, sizeof(UINT32)); 
	return true;
}

bool CProtocolCoder::Decode(const void* pcData, UINT32 dwLen, CProtocol& roPtc)
{
	m_oStream.Clear();

	m_oStream.Assign(pcData, dwLen);
	m_oStream >> roPtc;
	return true;
}

bool CProtocolCoder::GetProtocolType(const char* pcData, UINT32 dwLen, UINT32& rdwPtcType)
{
	if(pcData == NULL || dwLen < 8)
	{
		return false;
	}

	rdwPtcType = *(UINT32*)(pcData + 4);
	return true; 
}

bool CProtocolCoder::GetProtocolLen(const char* pcData, UINT32 dwLen, UINT32& rdwPtcLen)
{
	if(pcData == NULL || dwLen < 4)
	{
		return false;
	}

	rdwPtcLen = *(UINT32*)(pcData);
	return true; 
}

const char* CProtocolCoder::GetData() const
{
	return m_oStream.Data();
}

UINT32 CProtocolCoder::GetLenth() const
{
	return m_oStream.Size();
}