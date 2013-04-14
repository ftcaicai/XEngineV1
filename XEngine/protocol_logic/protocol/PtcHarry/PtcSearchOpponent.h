#ifndef _Ptc_SearchOpponent_H__
#define _Ptc_SearchOpponent_H__

#include "../Coder_Encoder/define.h"
#include "../Coder_Encoder/protocol.h"
#include "../buildingdata.h"


#include <vector>

class CSearchOpponentProtocol : public CProtocol
{
public:
	CSearchOpponentProtocol()
		:CProtocol(PROTOCOL_SearchOpponent_)
	{ 
	}

	CSearchOpponentProtocol(const CSearchOpponentProtocol& rhs)
		:CProtocol(rhs.m_dwType)
	{ 
	}

	virtual ~CSearchOpponentProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		return bs;
	}

};

class CAckSearchOpponentProtocol : public CProtocol
{
public:
	CAckSearchOpponentProtocol()
		:CProtocol(PROTOCOL_AckSearchOpponent_)
	{ 
	}

	CAckSearchOpponentProtocol(const CAckSearchOpponentProtocol& rhs)
		:CProtocol(rhs.m_dwType)
	{ 
	}

	virtual ~CAckSearchOpponentProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs  << m_iAccount;
		bs	<< m_strName;
		bs	<< m_oBuildings;
		bs	<< m_oStorageDatas;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs	>> m_iAccount;
		bs	>> m_strName;
		bs	>> m_oBuildings;
		bs	>> m_oStorageDatas;
		return bs;
	}

public:
	int							m_iAccount;
	std::string					m_strName;
	std::vector<BuildingData>	m_oBuildings;	
	std::vector<StorageData>    m_oStorageDatas;
};

#endif