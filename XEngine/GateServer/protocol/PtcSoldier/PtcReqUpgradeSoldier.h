#ifndef _Ptc_UpgradeSoldier_H__
#define _Ptc_UpgradeSoldier_H__

#include "../Coder_Encoder/define.h"
#include "../Coder_Encoder/protocol.h"



class CReqUpgradeSoldierProtocol : public CProtocol
{
public:
	CReqUpgradeSoldierProtocol()
		:CProtocol(PROTOCOL_ReqUpgradeSoldier_)
	{ 
	}

	CReqUpgradeSoldierProtocol(const CReqUpgradeSoldierProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iSoldierID(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CReqUpgradeSoldierProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iSoldierID;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iSoldierID;
		return bs;
	}

public:
	int m_iSoldierID;

};

class CAckUpgradeSoldierProtocol : public CProtocol
{
public:
	CAckUpgradeSoldierProtocol()
		:CProtocol(PROTOCOL_AckUpgradeSoldier_)
	{ 
	}

	CAckUpgradeSoldierProtocol(const CAckUpgradeSoldierProtocol& rhs)
		:CProtocol(rhs.m_dwType)		
		,m_iSoldierID(rhs.m_iSoldierID)
		,m_TimeBeginUpgrade(rhs.m_TimeBeginUpgrade)
		,m_TimeEndUpgrade(rhs.m_TimeEndUpgrade)
		,m_iOilCost(rhs.m_iOilCost)
	{ 
	}

	virtual ~CAckUpgradeSoldierProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iSoldierID;
		bs << m_TimeBeginUpgrade;
		bs << m_TimeEndUpgrade;
		bs << m_iOilCost;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iSoldierID;
		bs >> m_TimeBeginUpgrade;
		bs >> m_TimeEndUpgrade;
		bs >> m_iOilCost;
		return bs;
	}

public:
	int			m_iSoldierID;
	__time64_t	m_TimeBeginUpgrade;
	__time64_t	m_TimeEndUpgrade;
	int			m_iOilCost;

};


class CAckUpgradeSoldierSuccProtocol : public CProtocol
{
public:
	CAckUpgradeSoldierSuccProtocol()
		:CProtocol(PROTOCOL_AckUpgradeSoldierSucc_)
	{ 
	}

	CAckUpgradeSoldierSuccProtocol(const CAckUpgradeSoldierSuccProtocol& rhs)
		:CProtocol(rhs.m_dwType)		
		,m_iSoldierID(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CAckUpgradeSoldierSuccProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iSoldierID;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iSoldierID;
		return bs;
	}

public:
	int			m_iSoldierID;

};

#endif