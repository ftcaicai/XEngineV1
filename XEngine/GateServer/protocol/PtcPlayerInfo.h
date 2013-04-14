#ifndef _Ptc_PlayerInfo_H__
#define _Ptc_PlayerInfo_H__

#include "../protocol/Coder_Encoder/define.h"
#include "../protocol/Coder_Encoder/protocol.h"
#include "buildingdata.h"

#include <vector>



class CReqPlayerInfoProtocol : public CProtocol
{
public:
	CReqPlayerInfoProtocol()
		:CProtocol(PROTOCOL_ReqPlayerInfo_)
	{ 
	}

	CReqPlayerInfoProtocol(const CReqPlayerInfoProtocol& rhs)
		:CProtocol(rhs.m_dwType)
	{ 
	}

	virtual ~CReqPlayerInfoProtocol()
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

class CAckPlayerInfoProtocol : public CProtocol
{
public:
	CAckPlayerInfoProtocol()
		:CProtocol(PROTOCOL_AckPlayerInfo_)
	{ 
	}

	CAckPlayerInfoProtocol(const CAckPlayerInfoProtocol& rhs)
		:CProtocol(rhs.m_dwType)
	{ 
	}

	virtual ~CAckPlayerInfoProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs  << m_TimeNow;
		bs	<< m_strName;
		bs  << m_iRMB;
		bs  << m_iGold;
		bs	<< m_iOil; 
		bs  << m_iPopulation;
		bs	<< m_oBuildings;
		bs  << m_oBuilderDataToClient;
		bs  << m_labdata;
		bs	<< m_SoldierData;
		bs  << m_mapUniID2CampDataToClient;
		bs  << m_mapUniID2PopuStationData;
		bs	<< m_ProtectTime;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs  >> m_TimeNow;
		bs	>> m_strName;
		bs  >> m_iRMB;
		bs  >> m_iGold;
		bs	>> m_iOil; 
		bs  >> m_iPopulation;
		bs	>> m_oBuildings;
		bs  >> m_oBuilderDataToClient;
		bs  >> m_labdata;
		bs  >> m_SoldierData;
		bs  >> m_mapUniID2CampDataToClient;
		bs  >> m_mapUniID2PopuStationData;
		bs  >> m_ProtectTime;
		return bs;
	}

public:
	__time64_t							m_TimeNow;
	std::string							m_strName;
	int									m_iRMB;
	int									m_iGold;
	int									m_iOil;
	int									m_iPopulation;
	std::vector<BuildingData>			m_oBuildings;
	std::vector<CBuilderDataToClient>	m_oBuilderDataToClient;
	LabData								m_labdata;
	SoldierData							m_SoldierData;
	std::map<int,CCampDataToClient>		m_mapUniID2CampDataToClient;
	std::map<int,CPopStationData>		m_mapUniID2PopuStationData;
	__time64_t							m_ProtectTime;
};

#endif