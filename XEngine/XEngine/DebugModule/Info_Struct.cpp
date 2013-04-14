#include "Info_Struct.h"

#ifdef INFOLIST
#undef INFOLIST
#endif

#define INFOLIST(_Type,_Level,_Msg,_ShowMsg)\
{\
	_Type,\
	_Msg,\
	_Level,\
#_ShowMsg,\
},

stList g_stInfo[]=
{
#include "ListInfo.txt"
};