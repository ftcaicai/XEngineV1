#ifndef __PLAYER_WAR_REPORT_H__
#define __PLAYER_WAR_REPORT_H__
#include "../protocol/Coder_Encoder/pch.h"
#include "../protocol/Coder_Encoder/define.h"
#include "../protocol/system/data.h"

class CPlayer;
class CPlayerWarReportData : public CData
{
public:
	CPlayerWarReportData()
		:iIndex(0)
		,iAttacker_Level(0)
		,iRob_Money(0)
		,iRob_Oil(0)
		,iAttacker_Exp(0)
		,iSufferer_Exp(0)
		,iIsWin(0)
		,iIsTop(0)
		,WarTime(0)
		,iIsNewReport(0)
	{
		sSufferer_Name.clear();
		sAttacker_Name.clear();
		sAttacker_League.clear();
		sAttacker_Soilders.clear();
	}
	CPlayerWarReportData(const CPlayerWarReportData &rhs)
	:iIndex(rhs.iIndex)
	,iAttacker_ID(rhs.iAttacker_ID)
	,iAttacker_Level(rhs.iAttacker_Level)
	,iRob_Money(rhs.iRob_Money)
	,iRob_Oil(rhs.iRob_Oil)
	,iAttacker_Exp(rhs.iAttacker_Exp)
	,iSufferer_Exp(rhs.iSufferer_Exp)
	,iIsWin(rhs.iIsWin)
	,iIsTop(rhs.iIsTop)
	,WarTime(rhs.WarTime)
	,iIsNewReport(0)
	{
		sSufferer_Name = rhs.sSufferer_Name;
		sAttacker_Name = rhs.sAttacker_Name;
		sAttacker_League = rhs.sAttacker_League;
		sAttacker_Soilders = rhs.sAttacker_Soilders;
	}
	virtual ~CPlayerWarReportData()
	{

	}
	CPlayerWarReportData & operator = (const CPlayerWarReportData &rhs)
	{
		if(&rhs != this)
		{
			iIndex =rhs.iIndex;
			sSufferer_Name = rhs.sSufferer_Name;
			iAttacker_ID = rhs.iAttacker_ID;
			sAttacker_Name = rhs.sAttacker_Name;
			iAttacker_Level = rhs.iAttacker_Level;
			sAttacker_League = rhs.sAttacker_League;
			sAttacker_Soilders = rhs.sAttacker_Soilders;
			iRob_Money = rhs.iRob_Money;
			iRob_Oil = rhs.iRob_Oil;
			iAttacker_Exp = rhs.iAttacker_Exp;
			iSufferer_Exp = rhs.iSufferer_Exp;
			iIsWin = rhs.iIsWin;
			iIsTop = rhs.iIsTop;
			WarTime = rhs.WarTime;
			iIsNewReport = rhs.iIsNewReport;
		}
		return *this;
	}
	
protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{
		bs << iIndex;
		bs << sSufferer_Name;
		bs << iAttacker_ID;
		bs << sAttacker_Name;
		bs << iAttacker_Level;
		bs << sAttacker_League;
		bs << sAttacker_Soilders;
		bs << iRob_Money;
		bs << iRob_Oil;
		bs << iAttacker_Exp;
		bs << iSufferer_Exp;
		bs << iIsWin;
		bs << iIsTop;
		bs << WarTime;
		bs << iIsNewReport;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{
		bs >> iIndex;
		bs >> sSufferer_Name;
		bs >> iAttacker_ID;
		bs >> sAttacker_Name;
		bs >> iAttacker_Level;
		bs >> sAttacker_League;
		bs >> sAttacker_Soilders;
		bs >> iRob_Money;
		bs >> iRob_Oil;
		bs >> iAttacker_Exp;
		bs >> iSufferer_Exp;
		bs >> iIsWin;
		bs >> iIsTop;
		bs >> WarTime;
		bs >> iIsNewReport;
		return bs;
	}
public:
	int								iIndex;
	std::string						sSufferer_Name;
	int								iAttacker_ID;
	std::string						sAttacker_Name;
	int								iAttacker_Level;
	std::string						sAttacker_League;
	std::string						sAttacker_Soilders;
	int								iRob_Money;
	int								iRob_Oil;
	int								iAttacker_Exp;
	int								iSufferer_Exp;
	int								iIsWin;
	int								iIsTop;
	__time64_t						WarTime;
	int								iIsNewReport;
};
class CPlayerWarReport
{
public:
	CPlayerWarReport (CPlayer *	pPlayer);
	~CPlayerWarReport();
	
	void SetWarReport(std::vector<CPlayerWarReportData> cPlayerWarReport);
	std::vector<CPlayerWarReportData> GetWarReport(void);
private:
	std::vector<CPlayerWarReportData>						m_vPlayerWarReport;
	CPlayer *												m_pPlayer;
};
#endif