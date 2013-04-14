#ifndef _PTC_WARREPORT_H__
#define _PTC_WARREPORT_H__

#include "../protocol/Coder_Encoder/define.h"

#include "../protocol/Coder_Encoder/protocol.h"
#include "../PlayerMgr/PlayerWarReport.h"

class CPlayerWarReportProtocol : public CProtocol
{
public:
	CPlayerWarReportProtocol()
		:CProtocol(PROTOCOL_WarReport)
	{ 
	}

	CPlayerWarReportProtocol(const CPlayerWarReportProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_strName(rhs.m_strName)
	{ 
	}

	virtual ~CPlayerWarReportProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_strName;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_strName;
		return bs;
	}

public:
	std::string  m_strName;

};
class CPlayerWarReportACKProtocol : public CProtocol 
{
public:
	CPlayerWarReportACKProtocol()
		:CProtocol(PROTOCOL_AckWarReport)
	{
		m_vecData.clear();
	}
	CPlayerWarReportACKProtocol(const CPlayerWarReportACKProtocol &rhs)
		:CProtocol(rhs.m_dwType)
	{
		m_vecData = rhs.m_vecData;
	}
	virtual ~CPlayerWarReportACKProtocol()
	{

	}
	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_vecData;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_vecData;
		return bs;
	}
public:

	std::vector<CPlayerWarReportData> m_vecData;
};
class CPlayerWarReportUpdateProtocol : public CProtocol 
{
public:
	CPlayerWarReportUpdateProtocol()
		:CProtocol(PROTOCOL_UpdateWarReport)

	{

	}
	CPlayerWarReportUpdateProtocol(const CPlayerWarReportUpdateProtocol &rhs)
		:CProtocol(rhs.m_dwType)

	{

	}
	virtual ~CPlayerWarReportUpdateProtocol()
	{

	}
	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << data;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> data;
		return bs;
	}
public:
	CPlayerWarReportData data;
};

class CPlayerWarReportSetTopProtocol : public CProtocol 
{
public:
	CPlayerWarReportSetTopProtocol()
		:CProtocol(PROTOCOL_SetWarReportTop)

	{

	}
	CPlayerWarReportSetTopProtocol(const CPlayerWarReportSetTopProtocol &rhs)
		:CProtocol(rhs.m_dwType)

	{

	}
	virtual ~CPlayerWarReportSetTopProtocol()
	{

	}
	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << iIndex;
		bs << iIsTop;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> iIndex;
		bs >> iIsTop;
		return bs;
	}
public:
	int								iIndex;
	int								iIsTop;
};

class CPlayerWarReportDetailProtocol : public CProtocol 
{
public:
	CPlayerWarReportDetailProtocol()
		:CProtocol(PROTOCOL_SetWarReportDetail)

	{

	}
	CPlayerWarReportDetailProtocol(const CPlayerWarReportDetailProtocol &rhs)
		:CProtocol(rhs.m_dwType)

	{

	}
	virtual ~CPlayerWarReportDetailProtocol()
	{

	}
	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_iAccountID;
		bs << m_iLostBuildingPercent;
		bs << m_vecData;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_iAccountID;
		bs >> m_iLostBuildingPercent;
		bs >> m_vecData;
		return bs;
	}
public:
	int						 m_iAccountID;
	int						 m_iLostBuildingPercent;
	std::vector<StorageData> m_vecData;
};
#endif