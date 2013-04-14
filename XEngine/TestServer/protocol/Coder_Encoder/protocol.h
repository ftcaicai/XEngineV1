#ifndef _Protocol_H__
#define _Protocol_H__

#include "define.h"
#include "bytestream.h"


class CProtocol
{
public:
	explicit CProtocol(UINT32 dwType) : m_dwType(dwType)
	{ 
	}
	virtual ~CProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID) = 0;


	static bool Register(CProtocol* poProtocol);
	static void UnRegister();
	static CProtocol* GetProtocol(UINT32 dwType);


	friend ByteStream& operator << (ByteStream& bs, const CProtocol& roPtc);
	friend const ByteStream& operator >> (const ByteStream& bs, CProtocol& roPtc);

protected: 
	virtual ByteStream& Serialize(ByteStream& bs) const = 0;
	virtual const ByteStream& DeSerialize(const ByteStream& bs) = 0;

public:
	UINT32	m_dwType;

	typedef std::map<UINT32, CProtocol*>	CProtocolMap;
	static CProtocolMap	sm_oPtcMap;
};


inline ByteStream& operator << (ByteStream& bs, const CProtocol& roPtc)
{
	roPtc.Serialize(bs);
	return bs;
}

inline const ByteStream& operator >> (const ByteStream& bs, CProtocol& roPtc)
{
	roPtc.DeSerialize(bs);
	return bs;
}

#endif