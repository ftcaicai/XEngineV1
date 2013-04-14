#ifndef _Ptc_BuildUpgrade_H__
#define _Ptc_BuildUpgrade_H__

#include "../protocol/Coder_Encoder/define.h"

#include "../protocol/Coder_Encoder/protocol.h"

class CBuildUpgradeProtocol : public CProtocol
{ 
public:
	CBuildUpgradeProtocol()
		:CProtocol(PROTOCOL_BuildUpgrade_ID)
	{ 
	}

	CBuildUpgradeProtocol(const CBuildUpgradeProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
	{ 
	}

	virtual ~CBuildUpgradeProtocol()
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
		bs >> m_iUniID;
		return bs;
	}

public:
	int m_iUniID;

};

#endif