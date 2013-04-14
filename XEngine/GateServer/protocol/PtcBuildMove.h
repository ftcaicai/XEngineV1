#ifndef _Ptc_BuildMove_H__
#define _Ptc_BuildMove_H__

#include "../protocol/Coder_Encoder/define.h"

#include "../protocol/Coder_Encoder/protocol.h"




class CBuildMoveProtocol : public CProtocol
{ 
public:
	CBuildMoveProtocol()
		:CProtocol(PROTOCOL_BuildMoveID)
	{ 
	}

	CBuildMoveProtocol(const CBuildMoveProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iGridX(rhs.m_iGridX)
		,m_iGridY(rhs.m_iGridY)
	{ 
	}

	virtual ~CBuildMoveProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iGridX;
		bs << m_iGridY;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iGridX;
		bs >> m_iGridY;
		return bs;
	}

public:
	int m_iUniID;
	int m_iGridX;
	int m_iGridY;

};

#endif