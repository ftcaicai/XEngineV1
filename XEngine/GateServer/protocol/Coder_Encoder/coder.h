#ifndef _Protocol_Coder_H__
#define _Protocol_Coder_H__

#include "bytestream.h"
class CProtocol;


class CProtocolCoder
{
public:
	CProtocolCoder();
	~CProtocolCoder();


	bool Encode(const CProtocol& roPtc);
	bool Decode(const void* pcData, UINT32 dwLen, CProtocol& roPtc);


	bool GetProtocolType(const char* pcData, UINT32 dwLen, UINT32& rdwPtcType); 
	bool GetProtocolLen(const char* pcData, UINT32 dwLen, UINT32& rdwPtcLen);


	//获取Encode后的数据
	const char* GetData() const;
	UINT32 GetLenth() const;

protected:
	ByteStream	m_oStream;
};

#endif