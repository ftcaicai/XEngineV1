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

class CReqFinishBuildUpgradeRMBProtocol : public CProtocol
{ 
public:
	CReqFinishBuildUpgradeRMBProtocol()
		:CProtocol(PROTOCOL_ReqFinishBuildUpgradeRMB)
	{ 
	}

	CReqFinishBuildUpgradeRMBProtocol(const CReqFinishBuildUpgradeRMBProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iRMB(rhs.m_iRMB)
	{ 
	}

	virtual ~CReqFinishBuildUpgradeRMBProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iRMB;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iRMB;
		return bs;
	}

public:
	int m_iUniID;
	int m_iRMB;

};


#endif