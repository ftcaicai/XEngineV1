#ifndef _Ptc_AddResource_H__
#define _Ptc_AddResource_H__

#include "../protocol/Coder_Encoder/define.h"
#include "../protocol/Coder_Encoder/protocol.h"

#include <vector>

class CReqAddResourceProtocol : public CProtocol
{
public:
	CReqAddResourceProtocol()
		:CProtocol(PROTOCOL_ReqAddResource_)
	{ 
	}

	CReqAddResourceProtocol(const CReqAddResourceProtocol& rhs)
		:CProtocol(rhs.m_dwType)
	{ 
	}

	virtual ~CReqAddResourceProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << eResourceType;
		bs << iValue;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> eResourceType;
		bs >> iValue;
		return bs;
	}

	int	eResourceType;
	int	iValue;

};

class CAckAddResourceProtocol : public CProtocol
{
public:
	CAckAddResourceProtocol()
		:CProtocol(PROTOCOL_AckAddResource_)
	{ 
	}

	CAckAddResourceProtocol(const CAckAddResourceProtocol& rhs)
		:CProtocol(rhs.m_dwType)
	{ 
	}

	virtual ~CAckAddResourceProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs	<< eResourceType;
		bs  << iValue;
		bs	<< iResult;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs	>> eResourceType;
		bs  >> iValue;
		bs	>> iResult;
		return bs;
	}

public:
	int			 eResourceType;
	int			 iValue;
	int			 iResult;
};

#endif