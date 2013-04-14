#include"PlayerWarReport.h"
CPlayerWarReport::CPlayerWarReport(CPlayer *	pPlayer)
{
	m_pPlayer = pPlayer;
}
CPlayerWarReport::~CPlayerWarReport()
{

}
void CPlayerWarReport::SetWarReport(std::vector<CPlayerWarReportData> cPlayerWarReport)
{
	m_vPlayerWarReport = cPlayerWarReport;
}
std::vector<CPlayerWarReportData> CPlayerWarReport::GetWarReport(void)
{
	return m_vPlayerWarReport;
}
