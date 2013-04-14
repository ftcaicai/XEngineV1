#ifndef _Define_H__
#define _Define_H__

// define basic data types

typedef signed char  		INT8;
typedef signed short 		INT16;
typedef signed int	  		INT32;
typedef signed long long 	INT64;

typedef unsigned char 		BYTE;
typedef unsigned char 		UINT8;
typedef unsigned short 		UINT16;
typedef unsigned int 		UINT32;
typedef unsigned long long 	UINT64;


#define ASSERT(cond) assert(cond)

#define STRUCT_OFFSET(struct_name, elem_name) (unsigned long)(&((struct struct_name*)0)->elem_name)
#define STRUCT_HEADOF(elem_ptr, struct_name, elem_name) (struct_name*)((char*)elem_ptr - STRUCT_OFFSET(struct_name, elem_name)) 


struct CUserData
{
	CUserData()
		:m_dwUserData(0), m_pUserPtr(0)
	{ 
	}

	CUserData(UINT32 dwData, void* pPtr)
		:m_dwUserData(dwData), m_pUserPtr(pPtr)
	{ 
	}

	UINT32	m_dwUserData;
	void*	m_pUserPtr;
};


inline UINT16 MakeUINT16(UINT8 HiByte, UINT8 LoByte)
{
	return ((UINT16)HiByte << 8 | LoByte);
}

inline UINT32 MakeUINT32(UINT16 HiWord, UINT16 LoWord)
{
	return ((UINT32)HiWord << 16 | LoWord);
}

inline UINT64 MakeUINT64(UINT32 HiDWord, UINT32 LoDWord)
{
	return ((UINT64)HiDWord << 32 | LoDWord);
}

inline UINT8 GetHiByte(UINT16 wVal)
{
	return (UINT8)(wVal >> 8 & 0xFF);
}
 
inline UINT8 GetLoByte(UINT16 wVal)
{
	return (UINT8)(wVal & 0xFF);
}

inline UINT16 GetHiWord(UINT32 dwVal)
{
	return (UINT16)(dwVal >> 16 & 0xFFFF);
}

inline UINT16 GetLoWord(UINT32 dwVal)
{ 
	return (UINT16)(dwVal & 0xFFFF);
}

inline UINT32 GetHiDword(UINT64 qwVal)
{
	return (UINT32)(qwVal >> 32 & 0xFFFFFFFF);
}

inline UINT32 GetLoDword(UINT64 qwVal)
{
	return (UINT32)(qwVal & 0xFFFFFFFF); 
}

enum EResouceType
{
	eError	= 0,
	eRMB	= 1,
	eGold	= 2,
	eOil	= 3,
};


#define PROTOCOL_TEST_ID		1
#define PROTOCOL_REGISTER_ID	2
#define PROTOCOL_Login_ID		3
#define PROTOCOL_LoginACK_ID	4
#define PROTOCOL_ReqPlayerInfo_	5
#define PROTOCOL_AckPlayerInfo_	6
#define PROTOCOL_BuildAddID		7
#define PROTOCOL_BuildEraseID	8
#define PROTOCOL_BuildMoveID	9
#define PROTOCOL_BuildUpgrade_ID	10
#define PROTOCOL_BuildResourceCollect_ID	11
#define PROTOCOL_AckBuildResourceCollect_ID	12
#define PROTOCOL_AckBuildLevelUpBegin_ID	13
#define PROTOCOL_AckBuildLevelUpEnd_ID		14

#define PROTOCOL_SearchOpponent_		30
#define PROTOCOL_AckSearchOpponent_		31
#define PROTOCOL_HarryResource_ID		32
#define PROTOCOL_AckHarryResource_ID	33


#define PROTOCOL_ReqCreateSoldier_	40
#define PROTOCOL_AckCreateSoldier_	41
#define PROTOCOL_ReqOutputSoldier_	42
#define PROTOCOL_AckOutputSoldier_	43
#define PROTOCOL_ReqUpgradeSoldier_	45
#define PROTOCOL_AckUpgradeSoldier_	46
#define PROTOCOL_AckUpgradeSoldierSucc_	47
#define PROTOCOL_SyncPopulation_	48
#define	PROTOCOL_AckSoldierInCreating_ 49
#define	PROTOCOL_AckSoldierBeginCreate_ 50

#define	PROTOCOL_NotifyServerPutSoldier 60

#define PROTOCOL_ReqAddResource_	100
#define PROTOCOL_AckAddResource_	101

#endif
