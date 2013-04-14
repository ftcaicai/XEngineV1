#ifndef _Ptc_BuildErase_H__
#define _Ptc_BuildErase_H__

#include "../protocol/Coder_Encoder/define.h"

#include "../protocol/Coder_Encoder/protocol.h"




class CBuildEraseProtocol : public CProtocol
{ 
public:
	CBuildEraseProtocol()
		:CProtocol(PROTOCOL_BuildEraseID)
	{ 
	}

	CBuildEraseProtocol(const CBuildEraseProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
	{ 
	}

	virtual ~CBuildEraseProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >>  m_iUniID;;
		return bs;
	}

public:
	int m_iUniID;

};

#endif