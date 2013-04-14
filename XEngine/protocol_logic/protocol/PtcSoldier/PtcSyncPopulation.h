#ifndef _Ptc_SyncPopulation_H__
#define _Ptc_SyncPopulation_H__

#include "../Coder_Encoder/define.h"
#include "../Coder_Encoder/protocol.h"




class CSyncPopulationProtocol : public CProtocol
{
public:
	CSyncPopulationProtocol()
		:CProtocol(PROTOCOL_SyncPopulation_)
	{ 
	}

	CSyncPopulationProtocol(const CSyncPopulationProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iPopulation(rhs.m_iPopulation)
	{ 
	}

	virtual ~CSyncPopulationProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iPopulation;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iPopulation;
		return bs;
	}

public:
	int m_iPopulation;

};

#endif