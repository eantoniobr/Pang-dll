#include "Global.h"


#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

CCompressionPacket::CCompressionPacket()
{
	m_Buffer = new BYTE[128 * 1024];
	Init();
}

void CCompressionPacket::Init()
{
	__lzo_init_v2(MINILZO_VERSION, 2, 4, 4, 4, 4, 4, 4, 4, 24);
}

int CCompressionPacket::Compress(const unsigned char* pSrc, int srcSize)
{
	if (lzo_init() != LZO_E_OK)
	{
		printf("internal error - lzo_init() failed !!!\n");
		printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
		return 1;
	}

	int r;
	lzo_uint in_len = srcSize;
	static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
				
		lzo_uint DestSize;

		m_Buffer = new BYTE[128 * 1024];
		
		m_Buffer[3] = srcSize + srcSize / 255;
		m_Buffer[2] = ((srcSize - m_Buffer[3]) / 255) + ((srcSize - m_Buffer[3]) / 255) / 255;
		m_Buffer[1] = ((((srcSize - m_Buffer[3]) / 255) - m_Buffer[2]) / 255) + ((((srcSize - m_Buffer[3]) / 255) - m_Buffer[2]) / 255) / 255;
		
	
		r = lzo1x_1_compress((const unsigned char *)pSrc, in_len, &m_Buffer[4], &DestSize, wrkmem);
		if (r != LZO_E_OK)
		{
			printf("Failed to compress the buffer - Error %i\n", r);
			return 1;
		}

		m_iSize = (DWORD)DestSize + 4;
		return 0;
}

int CCompressionPacket::Decompress(const unsigned char* pSrc, int srcSize) 
{
	if (lzo_init() != LZO_E_OK)
	{
		printf("internal error - lzo_init() failed !!!\n");
		printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
		return 0;
	}

		lzo_uint out_len ;
		lzo_uint in_len = srcSize;
		int iRet;

		m_Buffer = new BYTE[128 * 1024];
		iRet = lzo1x_decompress(pSrc, in_len, m_Buffer, &out_len, NULL);
		m_iSize = out_len;
		if (iRet != LZO_E_OK)
		{
			printf("Failed to decompress the buffer!!!\n");
			return 2;
		}

		return out_len;

	return 0;
}

unsigned char* CCompressionPacket::GetNewBuffer()
{
	if (m_iSize)
	{
		return m_Buffer;
	}
	else
		return NULL;
}

int CCompressionPacket::Max(int a, int b)
{
	if (a <= b)
		return b;
	return a;
}

