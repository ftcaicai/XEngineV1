#ifndef _Building_Data_H__
#define _Building_Data_H__

#include "../protocol/Coder_Encoder/define.h"
#include "system/data.h"


class BuildingData : public CData
{
public:
	BuildingData()
		:m_nUniID(0)
		,m_nID(0)
		,m_nLevel(0)
		,m_nGridX(0)
		,m_nGridY(0)
		,m_bIsInLevelUp(false)
	{ 
	} 

	BuildingData(const BuildingData& rhs)
		:m_nUniID(rhs.m_nUniID)
		,m_nID(rhs.m_nID)
		,m_nLevel(rhs.m_nLevel)
		,m_nGridX(rhs.m_nGridX)
		,m_nGridY(rhs.m_nGridY)
		,m_bIsInLevelUp(rhs.m_bIsInLevelUp)
	{ 
	}

	virtual ~BuildingData()
	{
	}

	BuildingData& operator = (const BuildingData& rhs)
	{
		if(&rhs != this)
		{
			m_nUniID	= rhs.m_nUniID;
			m_nID	 = rhs.m_nID;
			m_nLevel = rhs.m_nLevel;
			m_nGridX = rhs.m_nGridX;
			m_nGridY = rhs.m_nGridY;
			m_bIsInLevelUp = rhs.m_bIsInLevelUp;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_nUniID;
		bs << m_nID;
		bs << m_nLevel;
		bs << m_nGridX;
		bs << m_nGridY;
		bs << m_bIsInLevelUp;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_nUniID;
		bs >> m_nID;
		bs >> m_nLevel;
		bs >> m_nGridX;
		bs >> m_nGridY;
		bs >> m_bIsInLevelUp;
		return bs;
	}

public:
	INT32		m_nUniID;
	INT32		m_nID;
	INT32		m_nLevel;
	INT32		m_nGridX;
	INT32		m_nGridY;
	bool		m_bIsInLevelUp;

};


class StorageData : public CData
{
public:
	StorageData()
		:m_nUniID(0)
		,m_nGoldStore(0)
		,m_nOilStore(0)

	{ 
	} 

	StorageData(const StorageData& rhs)
		:m_nUniID(rhs.m_nUniID)
		,m_nGoldStore(rhs.m_nGoldStore)
		,m_nOilStore(rhs.m_nOilStore)

	{ 
	}

	virtual ~StorageData()
	{
	}

	StorageData& operator = (const StorageData& rhs)
	{
		if(&rhs != this)
		{
			m_nUniID	 = rhs.m_nUniID;
			m_nGoldStore = rhs.m_nGoldStore;
			m_nOilStore  = rhs.m_nOilStore;

		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_nUniID;
		bs << m_nGoldStore;
		bs << m_nOilStore;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_nUniID;
		bs >> m_nGoldStore;
		bs >> m_nOilStore;
		return bs;
	}

public:
	INT32		m_nUniID;
	INT32		m_nGoldStore;
	INT32		m_nOilStore;

};

class LabData : public CData
{
public:
	LabData()
		:m_bUse(false)
		,m_iSoldierIDInUpgrade(0)
		,m_TimeBeginUpgrade(0)
		,m_TimeEndUpgrade(0)

	{ 
	} 

	LabData(const LabData& rhs)
		:m_bUse(rhs.m_bUse)
		,m_iSoldierIDInUpgrade(rhs.m_iSoldierIDInUpgrade)
		,m_TimeBeginUpgrade(rhs.m_TimeBeginUpgrade)
		,m_TimeEndUpgrade(rhs.m_TimeEndUpgrade)
		,m_mapSoldierID2Level(rhs.m_mapSoldierID2Level)

	{ 
	}

	virtual ~LabData()
	{
	}

	LabData& operator = (const LabData& rhs)
	{
		if(&rhs != this)
		{
			m_bUse					= rhs.m_bUse;
			m_iSoldierIDInUpgrade	= rhs.m_iSoldierIDInUpgrade;
			m_TimeBeginUpgrade		= rhs.m_TimeBeginUpgrade;
			m_TimeEndUpgrade		= rhs.m_TimeEndUpgrade;
			m_mapSoldierID2Level	= rhs.m_mapSoldierID2Level;

		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_bUse;
		bs << m_iSoldierIDInUpgrade;
		bs << m_TimeBeginUpgrade;
		bs << m_TimeEndUpgrade;
		bs << m_mapSoldierID2Level;

		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_bUse;
		bs >> m_iSoldierIDInUpgrade;
		bs >> m_TimeBeginUpgrade;
		bs >> m_TimeEndUpgrade;
		bs >> m_mapSoldierID2Level;
		return bs;
	}

public:
	int			m_bUse;
	int			m_iSoldierIDInUpgrade;
	__time64_t	m_TimeBeginUpgrade;
	__time64_t	m_TimeEndUpgrade;
	std::map<int,int> m_mapSoldierID2Level;

};


class SoldierData : public CData
{
public:
	SoldierData()
		:m_iSoldierCurrentPopulation(false)
	{ 
		m_mapSoldierList.clear();
	} 

	SoldierData(const SoldierData& rhs)
		:m_iSoldierCurrentPopulation(rhs.m_iSoldierCurrentPopulation)
		,m_mapSoldierList(rhs.m_mapSoldierList)
	{ 
	}

	virtual ~SoldierData()
	{
	}

	SoldierData& operator = (const SoldierData& rhs)
	{
		if(&rhs != this)
		{
			m_iSoldierCurrentPopulation	= rhs.m_iSoldierCurrentPopulation;
			m_mapSoldierList			= rhs.m_mapSoldierList;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_iSoldierCurrentPopulation;
		bs << m_mapSoldierList;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_iSoldierCurrentPopulation;
		bs >> m_mapSoldierList;
		return bs;
	}

public:
	int							m_iSoldierCurrentPopulation;
	std::map<int,int>			m_mapSoldierList;


};

class CampData;

class CampSData : public CData
{
public:
	CampSData()
	{ 
	} 

	CampSData(const CampSData& rhs)
		:m_mapID2CampData(rhs.m_mapID2CampData)
	{ 
	}

	virtual ~CampSData()
	{
	}

	CampSData& operator = (const CampSData& rhs)
	{
		if(&rhs != this)
		{
			m_mapID2CampData	= rhs.m_mapID2CampData;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_mapID2CampData;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_mapID2CampData;
		return bs;
	}

public:
	std::map<int,CampData> m_mapID2CampData;

};


class CampData : public CData
{
public:
	CampData()
	{ 
	} 

	CampData(const CampData& rhs)
		:m_bInCreating(rhs.m_bInCreating)
		,m_TimeEndCreating(rhs.m_TimeEndCreating)
		,m_iSoldierIDCreated(rhs.m_iSoldierIDCreated)
		,m_iPopulationInWaiting(rhs.m_iPopulationInWaiting)
		,m_vecSoldierWaitID(rhs.m_vecSoldierWaitID)
		,m_mapSoldierWaitNum(rhs.m_mapSoldierWaitNum)
	{ 
	}

	virtual ~CampData()
	{
	}

	CampData& operator = (const CampData& rhs)
	{
		if(&rhs != this)
		{
			m_bInCreating			=rhs.m_bInCreating;
			m_TimeEndCreating		=rhs.m_TimeEndCreating;
			m_iSoldierIDCreated		=rhs.m_iSoldierIDCreated;
			m_iPopulationInWaiting	=rhs.m_iPopulationInWaiting;
			m_vecSoldierWaitID		=rhs.m_vecSoldierWaitID;
			m_mapSoldierWaitNum		=rhs.m_mapSoldierWaitNum;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_bInCreating;
		bs << m_TimeEndCreating;
		bs << m_iSoldierIDCreated;
		bs << m_iPopulationInWaiting;
		bs << m_vecSoldierWaitID;
		bs << m_mapSoldierWaitNum;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_bInCreating;
		bs >> m_TimeEndCreating;
		bs >> m_iSoldierIDCreated;
		bs >> m_iPopulationInWaiting;
		bs >> m_vecSoldierWaitID;
		bs >> m_mapSoldierWaitNum;
		return bs;
	}

public:
	bool				m_bInCreating;
	__time64_t			m_TimeEndCreating;
	int					m_iSoldierIDCreated;
	int					m_iPopulationInWaiting;
	std::vector<int>	m_vecSoldierWaitID;
	std::map<int,int>	m_mapSoldierWaitNum;

};


class CCampDataToClient : public CData
{
public:
	CCampDataToClient()
	{ 
	} 

	CCampDataToClient(const CCampDataToClient& rhs)
		:m_iLevel(rhs.m_iLevel)
		,m_iSoldierIDCreated(rhs.m_iSoldierIDCreated)
		,m_TimeEndCreating(rhs.m_TimeEndCreating)
		,m_iPopulationInWaiting(rhs.m_iPopulationInWaiting)
		,m_iMaxPopulationWaiting(rhs.m_iMaxPopulationWaiting)
		,m_vecSoldierWaitID(rhs.m_vecSoldierWaitID)
		,m_mapSoldierWaitNum(rhs.m_mapSoldierWaitNum)
	{ 
	}

	virtual ~CCampDataToClient()
	{
	}

	CCampDataToClient& operator = (const CCampDataToClient& rhs)
	{
		if(&rhs != this)
		{
			m_iLevel				= rhs.m_iLevel;
			m_iSoldierIDCreated		= rhs.m_iSoldierIDCreated;
			m_TimeEndCreating		= rhs.m_TimeEndCreating;
			m_iPopulationInWaiting	= rhs.m_iPopulationInWaiting;
			m_iMaxPopulationWaiting	= rhs.m_iMaxPopulationWaiting;
			m_vecSoldierWaitID		= rhs.m_vecSoldierWaitID;
			m_mapSoldierWaitNum		= rhs.m_mapSoldierWaitNum;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_iLevel;
		bs << m_iSoldierIDCreated;
		bs << m_TimeEndCreating;
		bs << m_iPopulationInWaiting;
		bs << m_iMaxPopulationWaiting;
		bs << m_vecSoldierWaitID;
		bs << m_mapSoldierWaitNum;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_iLevel;
		bs >> m_iSoldierIDCreated;
		bs >> m_TimeEndCreating;
		bs >> m_iPopulationInWaiting;
		bs >> m_iMaxPopulationWaiting;
		bs >> m_vecSoldierWaitID;
		bs >> m_mapSoldierWaitNum;
		return bs;
	}

public:
	int			m_iLevel;	
	int			m_iSoldierIDCreated;
	__time64_t	m_TimeEndCreating;
	int			m_iPopulationInWaiting;
	int			m_iMaxPopulationWaiting;

	std::vector<int>	m_vecSoldierWaitID;
	std::map<int,int>	m_mapSoldierWaitNum;
};


class CBuilderData : public CData
{
public:
	CBuilderData()
	{ 
	} 

	CBuilderData(const CBuilderData& rhs)
		:m_iUniID(rhs.m_iUniID)
		,m_iUniIDInLevelUp(rhs.m_iUniIDInLevelUp)
		,m_iLevelInLevelUp(rhs.m_iLevelInLevelUp)
		,m_TimeEnd(rhs.m_TimeEnd)
	{ 
	}

	virtual ~CBuilderData()
	{
	}

	CBuilderData& operator = (const CBuilderData& rhs)
	{
		if(&rhs != this)
		{
			m_iUniID				= rhs.m_iUniID;
			m_iUniIDInLevelUp		= rhs.m_iUniIDInLevelUp;
			m_iLevelInLevelUp		= rhs.m_iLevelInLevelUp;
			m_TimeEnd				= rhs.m_TimeEnd;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_iUniID;
		bs << m_iUniIDInLevelUp;
		bs << m_iLevelInLevelUp;
		bs << m_TimeEnd;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_iUniID;
		bs >> m_iUniIDInLevelUp;
		bs >> m_iLevelInLevelUp;
		bs >> m_TimeEnd;
		return bs;
	}

public:
	int			m_iUniID;	
	int			m_iUniIDInLevelUp;
	int			m_iLevelInLevelUp;
	__time64_t	m_TimeEnd;
};


class CBuilderDataToClient : public CData
{
public:
	CBuilderDataToClient()
	{ 
	} 

	CBuilderDataToClient(const CBuilderDataToClient& rhs)
		:m_iUniID(rhs.m_iUniID)
		,m_iUniIDInLevelUp(rhs.m_iUniIDInLevelUp)
		,m_TimeEnd(rhs.m_TimeEnd)
	{ 
	}

	virtual ~CBuilderDataToClient()
	{
	}

	CBuilderDataToClient& operator = (const CBuilderDataToClient& rhs)
	{
		if(&rhs != this)
		{
			m_iUniID				= rhs.m_iUniID;
			m_iUniIDInLevelUp		= rhs.m_iUniIDInLevelUp;
			m_TimeEnd				= rhs.m_TimeEnd;
		}

		return *this;
	}

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << m_iUniID;
		bs << m_iUniIDInLevelUp;
		bs << m_TimeEnd;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> m_iUniID;
		bs >> m_iUniIDInLevelUp;
		bs >> m_TimeEnd;
		return bs;
	}

public:
	int			m_iUniID;	
	int			m_iUniIDInLevelUp;
	__time64_t	m_TimeEnd;
};

#endif