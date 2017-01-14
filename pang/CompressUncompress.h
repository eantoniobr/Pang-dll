#ifndef __COMPRESSUNCOMPRESS_H__
#define __COMPRESSUNCOMPRESS_H__


class CCompressionPacket
{
public:
	CCompressionPacket();
	virtual ~CCompressionPacket(){};
	void Init();

	unsigned char* GetNewBuffer();
	INT GetNewSize(){ return m_iSize; };
	int Compress(const unsigned char* pSrc, int srcSize);
	int Decompress(const unsigned char* pSrc, int srcSize);


private:
	int Max(int a, int b);

private:
	BYTE *m_Buffer;				
	bool m_bLzoUsed;	
	int m_bInitialized;	
	int m_iSize;
};

#endif // __COMPRESSUNCOMPRESS_H__