#ifndef _Data_H__
#define _Data_H__

#include "../Coder_Encoder/define.h"
#include "../Coder_Encoder/bytestream.h"

class CData
{
public:
	CData() {}
	virtual ~CData() {} 

	virtual void  Release()
	{
		delete this;
	}


	friend ByteStream& operator << (ByteStream& bs, const CData& roData);
	friend const ByteStream& operator >> (const ByteStream& bs, CData& roData);

protected:
	virtual ByteStream& Serialize(ByteStream& bs) const = 0;
	virtual const ByteStream& DeSerialize(const ByteStream& bs) = 0;
};


inline ByteStream& operator << (ByteStream& bs, const CData& roData)
{
	roData.Serialize(bs);
	return bs;
}

inline const ByteStream& operator >> (const ByteStream& bs, CData& roData)
{
	roData.DeSerialize(bs);
	return bs;
}

#endif