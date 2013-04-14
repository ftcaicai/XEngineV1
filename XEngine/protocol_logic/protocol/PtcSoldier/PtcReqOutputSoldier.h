#ifndef _Ptc_OutputSoldier_H__
#define _Ptc_OutputSoldier_H__

#include "../Coder_Encoder/define.h"
#include "../Coder_Encoder/protocol.h"




class CReqOutputSoldierProtocol : public CProtocol
{
public:
	CReqOutputSoldierProtocol()
		:CProtocol(PROTOCOL_ReqOutputSoldier_)
	{ 
	}

	CReqOutputSoldierProtocol(const CReqOutputSoldierProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iSoldierID(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CReqOutputSoldierProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iSoldierID;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iSoldierID;
		return bs;
	}

public:
	int m_iUniID;
	int m_iSoldierID;

};

class CAckOutputSoldierProtocol : public CProtocol
{
public:
	CAckOutputSoldierProtocol()
		:CProtocol(PROTOCOL_AckOutputSoldier_)
	{ 
	}

	CAckOutputSoldierProtocol(const CAckOutputSoldierProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iSoldierID(rhs.m_iSoldierID)
		,m_iCurSoldierPopulaiton(rhs.m_iCurSoldierPopulaiton)
	{ 
	}

	virtual ~CAckOutputSoldierProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iSoldierID;
		bs << m_iCurSoldierPopulaiton;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iSoldierID;
		bs >> m_iCurSoldierPopulaiton;
		return bs;
	}

public:
	int m_iUniID;
	int m_iSoldierID;
	int	m_iCurSoldierPopulaiton;

};



#endif