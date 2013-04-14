#ifndef _Ptc_RegisterPlayer_H__
#define _Ptc_RegisterPlayer_H__

#include "../protocol/Coder_Encoder/define.h"

#include "../protocol/Coder_Encoder/protocol.h"




class CRegisterPlayerProtocol : public CProtocol
{ 
public:
	CRegisterPlayerProtocol()
		:CProtocol(PROTOCOL_REGISTER_ID)
	{ 
	}

	CRegisterPlayerProtocol(const CRegisterPlayerProtocol& rhs)
		:CProtocol(rhs.m_dwType)
		,m_strPassWord(rhs.m_strPassWord)
		,m_strName(rhs.m_strName)
	{ 
	}

	virtual ~CRegisterPlayerProtocol()
	{ 
	}

	virtual void Process(UINT32 dwConnID);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const
	{ 
		bs << m_strPassWord;
		bs << m_strName;
		return bs;
	}

	virtual const ByteStream& DeSerialize(const ByteStream& bs)
	{ 
		bs >> m_strPassWord;
		bs >> m_strName;
		return bs;
	}

public:
	std::string  m_strPassWord;
	std::string  m_strName;

};

#endif