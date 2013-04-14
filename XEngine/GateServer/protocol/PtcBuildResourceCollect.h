#ifndef _Ptc_BuildResourceCollect_H__
#define _Ptc_BuildResourceCollect_H__

#include "Coder_Encoder/define.h"

#include "Coder_Encoder/protocol.h"



class CBuildResourceCollectProtocol : public CProtocol
{ 
public:
	CBuildResourceCollectProtocol()
		:CProtocol(PROTOCOL_BuildResourceCollect_ID)
	{ 
	}

	CBuildResourceCollectProtocol(const CBuildResourceCollectProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iGold(rhs.m_iGold)
		,m_iOil(rhs.m_iOil)
	{ 
	}

	virtual ~CBuildResourceCollectProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iGold;
		bs << m_iOil;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iGold;
		bs >> m_iOil;
		return bs;
	}

public:
	int m_iUniID;
	int m_iGold;
	int m_iOil;	

};

class CAckBuildResourceCollectProtocol : public CProtocol
{ 
public:
	CAckBuildResourceCollectProtocol()
		:CProtocol(PROTOCOL_AckBuildResourceCollect_ID)
	{ 
	}

	CAckBuildResourceCollectProtocol(const CAckBuildResourceCollectProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iGold(rhs.m_iGold)
		,m_iOil(rhs.m_iOil)
	{ 
	}

	virtual ~CAckBuildResourceCollectProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iGold;
		bs << m_iOil;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iGold;
		bs >> m_iOil;
		return bs;
	}

public:
	int m_iUniID;
	int m_iGold;
	int m_iOil;	

};

#endif