#ifndef _Ptc_Login_H__
#define _Ptc_Login_H__

#include "../protocol/Coder_Encoder/define.h"
//#include "data.h"
#include "../protocol/Coder_Encoder/protocol.h"
#include "testdata.h"
#include "buildingdata.h"

#include <vector>

class CLoginProtocol : public CProtocol
{
public:
	CLoginProtocol()
		:CProtocol(PROTOCOL_Login_ID)
	{ 
	}

	CLoginProtocol(const CLoginProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_strPassWord(rhs.m_strPassWord)
		,m_strName(rhs.m_strName)
	{ 
	}

	virtual ~CLoginProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_strPassWord;
		bs << m_strName;


		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_strPassWord;
		bs >> m_strName;


		return bs;
	}

public:
	std::string  m_strPassWord;
	std::string  m_strName;

};

class CLoginACKProtocol : public CProtocol
{
public:
	CLoginACKProtocol()
		:CProtocol(PROTOCOL_LoginACK_ID)
	{ 
	}

	CLoginACKProtocol(const CLoginACKProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_iRetNum(rhs.m_iRetNum)
	{ 
	}

	virtual ~CLoginACKProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_iRetNum;
		bs << m_oBuildings;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_iRetNum;
		bs >> m_oBuildings;
		return bs;
	}

public:
	INT32        m_iRetNum;
	std::vector<BuildingData>	m_oBuildings;
};

#endif