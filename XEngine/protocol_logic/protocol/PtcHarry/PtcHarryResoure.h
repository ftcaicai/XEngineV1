#ifndef _Ptc_HarryResource_H__
#define _Ptc_HarryResource_H__

#include "../Coder_Encoder/define.h"

#include "../Coder_Encoder/protocol.h"



class CHarryResourceProtocol : public CProtocol
{ 
public:
	CHarryResourceProtocol()
		:CProtocol(PROTOCOL_HarryResource_ID)
	{ 
	}

	CHarryResourceProtocol(const CHarryResourceProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iGold(rhs.m_iGold)
		,m_iOil(rhs.m_iOil)
	{ 
	}

	virtual ~CHarryResourceProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iGold;
		bs << m_iOil;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iGold;
		bs >> m_iOil;
		return bs;
	}

public:
	int m_iGold;
	int m_iOil;	

};


class CAckHarryResourceProtocol : public CProtocol
{ 
public:
	CAckHarryResourceProtocol()
		:CProtocol(PROTOCOL_AckHarryResource_ID)
	{ 
	}

	CAckHarryResourceProtocol(const CAckHarryResourceProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iGold(rhs.m_iGold)
		,m_iOil(rhs.m_iOil)
	{ 
	}

	virtual ~CAckHarryResourceProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iGold;
		bs << m_iOil;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iGold;
		bs >> m_iOil;
		return bs;
	}

public:
	int m_iGold;
	int m_iOil;	

};


#endif