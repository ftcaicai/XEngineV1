#ifndef _Item_Data_H__
#define _Item_Data_H__

#include "Coder_Encoder/define.h"
#include "system/data.h"


class ItemData : public CData
{
public:
	ItemData()
		:m_nID(0)
		,m_wCount(0)
		,m_fPrice(0.0f)
	{ 
	} 

	ItemData(const ItemData& rhs)
		:m_nID(rhs.m_nID)
		,m_wCount(rhs.m_wCount)
		,m_fPrice(rhs.m_fPrice)
		,m_strName(rhs.m_strName)
	{ 
	}

	virtual ~ItemData()
	{
	}

	ItemData& operator = (const ItemData& rhs)
	{
		if(&rhs != this)
		{
			m_nID = rhs.m_nID;
			m_wCount = rhs.m_wCount;
			m_fPrice = rhs.m_fPrice;
			m_strName = rhs.m_strName;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_nID;
		bs << m_wCount;
		bs << m_fPrice;
		bs << m_strName;

		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_nID;
		bs >> m_wCount;
		bs >> m_fPrice;
		bs >> m_strName;

		return bs;
	}

public:
	INT32		m_nID;
	UINT16		m_wCount;
	float		m_fPrice;
	std::string m_strName;
};

#endif