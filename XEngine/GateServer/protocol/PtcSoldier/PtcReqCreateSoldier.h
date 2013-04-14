#ifndef _Ptc_CreateSoldier_H__
#define _Ptc_CreateSoldier_H__

#include "../Coder_Encoder/pch.h"
#include "../Coder_Encoder/protocol.h"




class CReqCreateSoldierProtocol : public CProtocol
{
public:
	CReqCreateSoldierProtocol()
		:CProtocol(PROTOCOL_ReqCreateSoldier_)
	{ 
	}

	CReqCreateSoldierProtocol(const CReqCreateSoldierProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iSoldierID(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CReqCreateSoldierProtocol()
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

class CAckCreateSoldierProtocol : public CProtocol
{
public:
	CAckCreateSoldierProtocol()
		:CProtocol(PROTOCOL_AckCreateSoldier_)
	{ 
	}

	CAckCreateSoldierProtocol(const CAckCreateSoldierProtocol& rhs)
		:CProtocol(rhs.m_dwType)		
		,m_iUniID(rhs.m_iUniID)
		,m_iSoldierID(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CAckCreateSoldierProtocol()
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


class CAckSoldierInCreatingProtocol : public CProtocol
{
public:
	CAckSoldierInCreatingProtocol()
		:CProtocol(PROTOCOL_AckSoldierInCreating_)
	{ 
	}

	CAckSoldierInCreatingProtocol(const CAckSoldierInCreatingProtocol& rhs)
		:CProtocol(rhs.m_dwType)		
		,m_iUniID(rhs.m_iUniID)
		,m_iSoldierID(rhs.m_iSoldierID)
		,m_iOilCost(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CAckSoldierInCreatingProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iSoldierID;
		bs << m_iOilCost;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iSoldierID;
		bs >> m_iOilCost;
		return bs;
	}

public:
	int m_iUniID;
	int m_iSoldierID;
	int m_iOilCost;

};


class CAckSoldierBeginCreateProtocol : public CProtocol
{
public:
	CAckSoldierBeginCreateProtocol()
		:CProtocol(PROTOCOL_AckSoldierBeginCreate_)
	{ 
	}

	CAckSoldierBeginCreateProtocol(const CAckSoldierBeginCreateProtocol& rhs)
		:CProtocol(rhs.m_dwType)		
		,m_iUniID(rhs.m_iUniID)
		,m_TimeEnd(rhs.m_TimeEnd)
	{ 
	}

	virtual ~CAckSoldierBeginCreateProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_TimeEnd;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_TimeEnd;
		return bs;
	}

public:
	int m_iUniID;
	__time64_t m_TimeEnd;

};

class CNotifyServerPutSoldierProtocol : public CProtocol
{
public:
	CNotifyServerPutSoldierProtocol()
		:CProtocol(PROTOCOL_NotifyServerPutSoldier)
	{ 
	}

	CNotifyServerPutSoldierProtocol(const CNotifyServerPutSoldierProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iSoldierID(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CNotifyServerPutSoldierProtocol()
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
class CReqCancelSoldierCreateProtocol : public CProtocol
{
public:
	CReqCancelSoldierCreateProtocol()
		:CProtocol(PROTOCOL_ReqCancelSoldierCreate)
	{ 
	}

	CReqCancelSoldierCreateProtocol(const CReqCancelSoldierCreateProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iSoldierID(rhs.m_iSoldierID)
	{ 
	}

	virtual ~CReqCancelSoldierCreateProtocol()
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
#endif