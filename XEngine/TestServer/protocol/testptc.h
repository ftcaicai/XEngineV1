#ifndef _Ptc_Test_H__
#define _Ptc_Test_H__

#include "Coder_Encoder/define.h"
//#include "data.h"
#include "Coder_Encoder/protocol.h"
#include "testdata.h"

class CTestProtocol : public CProtocol
{
public:
	CTestProtocol()
		:CProtocol(PROTOCOL_TEST_ID)
		,m_nID(0)
	{ 
	}

	CTestProtocol(const CTestProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_nID(rhs.m_nID)
		,m_strText(rhs.m_strText)
		//,m_oData(rhs.m_oData)
	{ 
	}

	virtual ~CTestProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_nID;
		bs << m_strText;
		//bs << m_oData;
		bs << m_bA;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_nID;
		bs >> m_strText;
		//bs >> m_oData;
		bs >> m_bA;
		return bs;
	}

public:
	INT32        m_nID;
	std::string  m_strText;
	//CTestData    m_oData;
	bool		 m_bA;
};

#endif