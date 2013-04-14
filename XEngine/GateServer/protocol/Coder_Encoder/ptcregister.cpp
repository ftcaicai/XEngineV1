#include "pch.h"
#include "ptcregister.h"

#include "../../protocol/testptc.h"
#include "../../protocol/PtcRegisterPlayer.h"
#include "../../protocol/PtcLogin.h"
#include "../../protocol/PtcPlayerInfo.h"

#include "../../protocol/PtcBuildAdd.h"
#include "../../protocol/PtcBuildErase.h"
#include "../../protocol/PtcBuildMove.h"
#include "../../protocol/PtcBuildResourceCollect.h"
#include "../../protocol/PtcBuildUpgrade.h"

#include "../../protocol/PtcHarry/PtcSearchOpponent.h"
#include "../../protocol/PtcHarry/PtcHarryResoure.h"

#include "../../protocol/PtcSoldier/PtcReqCreateSoldier.h"
#include "../../protocol/PtcSoldier/PtcReqOutputSoldier.h"
#include "../../protocol/PtcSoldier/PtcReqUpgradeSoldier.h"

#include "../../protocol/PtcWarReport/PtcWarReport.h"

void CProtocolRegister::Regiter()
{
	 CProtocol::Register(new CTestProtocol());

	 CProtocol::Register(new CRegisterPlayerProtocol());
	 CProtocol::Register(new CLoginProtocol());
	 CProtocol::Register(new CReqPlayerInfoProtocol());

	 CProtocol::Register(new CBuildAddProtocol());
	 CProtocol::Register(new CBuildEraseProtocol());
	 CProtocol::Register(new CBuildMoveProtocol());
	 CProtocol::Register(new CBuildResourceCollectProtocol());
	 CProtocol::Register(new CBuildUpgradeProtocol());

	 CProtocol::Register(new CReqFinishBuildUpgradeRMBProtocol());
	 CProtocol::Register(new CSearchOpponentProtocol());
	 CProtocol::Register(new CHarryResourceProtocol());

	 CProtocol::Register(new CReqCreateSoldierProtocol());
	 CProtocol::Register(new CReqOutputSoldierProtocol());

	 CProtocol::Register(new CReqUpgradeSoldierProtocol());

	 CProtocol::Register(new CReqCancelSoldierCreateProtocol());

	 CProtocol::Register(new CNotifyServerPutSoldierProtocol());

	 
	 CProtocol::Register(new CPlayerWarReportProtocol());
	 CProtocol::Register(new CPlayerWarReportUpdateProtocol());
	 CProtocol::Register(new CPlayerWarReportSetTopProtocol());
	 CProtocol::Register(new CPlayerWarReportDetailProtocol());
	 

}