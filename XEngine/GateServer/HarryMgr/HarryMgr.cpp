#include "HarryMgr.h"

INSTANCE_SINGLETON(CHarryMgr)

#include "../Interface/Protocol_logic.h"
//#include "../network.h"
#include "../MySQL/CMysql.h"

#include "../protocol/PtcHarry/PtcSearchOpponent.h"

//#include "../clientlink.h"

#include "../PlayerMgr/PlayerMgr.h"
#include "../PlayerMgr/Player.h"

CHarryMgr::CHarryMgr()
{ 
}

CHarryMgr::~CHarryMgr()
{ 
}

bool CHarryMgr::Init()
{
	return true;
}


void CHarryMgr::SearchOpponentForPlayer(UINT32 dwConnID)
{
	// dec search gold
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer==NULL)
	{
		//���յ��� ��Щ�ط����ǲ�Ӧ�ó���� ������� Ҫôbug Ҫô���
		return;
	}

	if(	pPlayer->DecSearchGold() == false)
	{
		//���յ��� ��Щ�ط����ǲ�Ӧ�ó���� ������� Ҫôbug Ҫô���
		return;
	}

	CMysql::Instance()->SearchOpponent(dwConnID);
}

				
void CHarryMgr::OnFindOpponent(UINT32 dwConnID, int iAccountID,std::string strName, const std::vector<BuildingData> & vecBuildings,const std::vector<StorageData> & vecStorageDatas)
{
	CAckSearchOpponentProtocol ptc; //= new CAckSearchOpponentProtocol();
	ptc.m_iAccount = iAccountID;
	ptc.m_strName = strName;
	ptc.m_oBuildings = vecBuildings;
	ptc.m_oStorageDatas = vecStorageDatas;
	
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(dwConnID,ptc);
	//CClientLink::Instance()->Send(dwConnID, ptc);
}