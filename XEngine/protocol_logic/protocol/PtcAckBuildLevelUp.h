#ifndef _Ptc_BuildUpgrade_H__
#define _Ptc_BuildUpgrade_H__

#include "../protocol/Coder_Encoder/define.h"

#include "../protocol/Coder_Encoder/protocol.h"




class CAckBuildLevelUpBeginProtocol : public CProtocol
{ 
public:
	CAckBuildLevelUpBeginProtocol()
		:CProtocol(PROTOCOL_AckBuildLevelUpBegin_ID)
	{ 
	}

	CAckBuildLevelUpBeginProtocol(const CAckBuildLevelUpBeginProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iUniIDInLevelUp(rhs.m_iUniIDInLevelUp)
		,m_timeEnd(rhs.m_timeEnd)
	{ 
	}

	virtual ~CAckBuildLevelUpBeginProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iUniIDInLevelUp;
		bs << m_timeEnd;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iUniIDInLevelUp;
		bs >> m_timeEnd;
		return bs;
	}

public:
	int			m_iUniID;
	int			m_iUniIDInLevelUp;
	__time64_t	m_timeEnd;
	
};

class CAckBuildLevelUpEndProtocol : public CProtocol
{ 
public:
	CAckBuildLevelUpEndProtocol()
		:CProtocol(PROTOCOL_AckBuildLevelUpEnd_ID)
	{ 
	}

	CAckBuildLevelUpEndProtocol(const CAckBuildLevelUpEndProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iUniID(rhs.m_iUniID)
		,m_iUniIDInLevelUp(rhs.m_iUniIDInLevelUp)
		,m_iLevel(rhs.m_iLevel)
	{ 
	}

	virtual ~CAckBuildLevelUpEndProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iUniID;
		bs << m_iUniIDInLevelUp;
		bs << m_iLevel;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iUniID;
		bs >> m_iUniIDInLevelUp;
		bs >> m_iLevel;
		return bs;
	}

public:
	int m_iUniID;
	int	m_iUniIDInLevelUp;
	int m_iLevel;
};


#endif