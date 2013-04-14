#ifndef _Test_Data_H__
#define _Test_Data_H__

#include "Coder_Encoder/define.h"
//#include "data.h"
#include "itemdata.h"


class CTestData : public CData
{
public:
	CTestData()
		:m_cID(0)
	{ 
	} 

	CTestData(const CTestData& rhs)
		:m_cID(rhs.m_cID)
		,m_oIDList(rhs.m_oIDList)
		,m_oItems(rhs.m_oItems)
	{ 
	}

	virtual ~CTestData()
	{
	}

	CTestData& operator = (const CTestData& rhs)
	{
		if(&rhs != this)
		{
			m_cID = rhs.m_cID;
			m_oIDList = rhs.m_oIDList;
			m_oItems = rhs.m_oItems;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_cID;
		bs << m_oIDList;
		bs << m_oItems;

		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_cID;
		bs >> m_oIDList;
		bs >> m_oItems;

		return bs;
	}

public: 
	INT8					m_cID;
	std::vector<UINT32>     m_oIDList;
	std::vector<ItemData>	m_oItems;
};

#endif