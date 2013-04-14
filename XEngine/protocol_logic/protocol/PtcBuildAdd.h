#ifndef _Ptc_BuildAdd_H__
#define _Ptc_BuildAdd_H__

#include "../protocol/Coder_Encoder/define.h"

#include "../protocol/Coder_Encoder/protocol.h"




class CBuildAddProtocol : public CProtocol
{ 
public:
	CBuildAddProtocol()
		:CProtocol(PROTOCOL_BuildAddID)
	{ 
	}

	CBuildAddProtocol(const CBuildAddProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iID(rhs.m_iID)
		,m_iLevel(rhs.m_iLevel)
		,m_iGridX(rhs.m_iGridX)
		,m_iGridY(rhs.m_iGridY)
	{ 
	}

	virtual ~CBuildAddProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iID;
		bs << m_iLevel;
		bs << m_iGridX;
		bs << m_iGridY;

		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iID;
		bs >> m_iLevel;
		bs >> m_iGridX;
		bs >> m_iGridY;

		return bs;
	}

public:
	int m_iUniID;
	int m_iID;
	int m_iLevel;
	int m_iGridX;
	int m_iGridY;

};

#endif