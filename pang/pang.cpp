// pang.cpp : Defines the exported functions for the DLL application.
//

#include "Global.h"
#include "Pang.h"


char ConvShortToChar(unsigned short int const num)
{
	string str = to_string(num);
	return str[0];
}

void string_copy(unsigned char *string1, unsigned char *string2, unsigned int size)
{
	for (unsigned int i = 0; i <= size; i++)
	{
		string1[i] = string2[i];
	}
}

int Encrypt_Packet(unsigned char *src, unsigned char *tar, int len, unsigned int key)
{
	unsigned char tmp[1024*500];

	memcpy(tmp, src, len);
	
	for (int i = 8; i < len; i++)
	{
		src[i] ^= tmp[i - 4];
	}
	src[4] ^= PublicKeyTable[key + src[0]];
	
	

	return 1;
}

int Decrypt_Packet(unsigned char *src, unsigned char *tar, int len, unsigned int key)
{
	
	unsigned char _tmp;

	src[4] = PublicKeyTable[key + src[0] + 1];

	
	for (int i = 8; i < 12; i++)
	{
		_tmp = src[i - 4];
		_tmp ^= src[i];
		src[i] = _tmp;
	}

	
	for (int i = 8; i < len; i++)
	{
		_tmp = src[i]; 
		_tmp ^= src[i + 4];
		src[i + 4] = _tmp;
	}

	src[4] ^= PublicKeyTable[key + src[0]];
	src[5] ^= 0x00;
	src[6] ^= 0x00;
	src[7] ^= 0x00;
	


	return 1;
}

// Decrypt pangya client packets
DLL_EXPORT int _pangya_client_decrypt(char *buffin, int size, char **buffout, int *buffoutSize, char key)
{
	// buffin is the pointer of Data received by the Pangya Client.
	// That mean, when you have a packet from the client, you must give it to this function.
	// This function should only know how to decrypt a single packet

	// "pangya_client_decrypt" must accept the full packet send by the client as data and must return the decrypted packet starting with the Id of the packet
	
	//memmove(*buffout, buffin, size);

	unsigned char *packet_decrypt;
	packet_decrypt = (unsigned char*)malloc(size);
	memmove(packet_decrypt, buffin, size);

	string size2;

	
	printd("************ _pangya_client_decrypt ************\n", 1);
	printd("New Incoming Call at :", 1);
	printd(GetDateTime().c_str(), 1);
	printd("\n", 1);
	printd("******************* Input *********************\n", 1);
	printd("Size : ", 1);
	size2 = to_string(size);
	printd(size2.c_str(), 1);
	printd("\n", 1);
	printd("Key(HEX) : ", 1);
	printd(ShowPacketInHex(&key, 2).c_str(), 1);
	printd("\n", 1);
	printf("%s\n", hex2ascii(PacketInHex(buffin, size)).c_str());
	printd("Data (HEX) :", 1);
	printd(ShowPacketInHex(buffin, size).c_str(), 1);
	printd("\n", 1);
	printd("***********************************************\n", 1);
	key = 0x21;

	int ParseKey =  key << 8 ;
	Decrypt_Packet(packet_decrypt, packet_decrypt, size, ParseKey);

	
	
	//if (packet_decrypt[size - 3] == 0x11)
	{
		size = size - 5;

		memcpy(&packet_decrypt[0], &packet_decrypt[5], size);
	}
	/*
	else
	{
		int NewSize = size - 4;
		unsigned char *tempPacket;
		tempPacket = (unsigned char*)malloc(NewSize);
		memcpy(tempPacket, (char*)packet_decrypt[4], NewSize);
		CCompressionPacket CompressionPacket;
		int NewSize2 = CompressionPacket.Decompress(tempPacket, NewSize);
		unsigned char *tempPacket2 = CompressionPacket.GetNewBuffer();
		memcpy(&packet_decrypt[0], &tempPacket2[0], NewSize2);
		size = NewSize2;
	}
	*/
	
	
	*buffoutSize = size;
	*buffout = (char*)malloc(*buffoutSize);
	memmove(*buffout, &packet_decrypt[0], *buffoutSize);

	// work finish here

	printd("******************* OutPut *********************\n", 1);
	printd("Size : ", 1);
	size2 = to_string(*buffoutSize);
	printd(size2.c_str(), 1);
	printd("\n", 1);
	printd("Data (HEX) :", 1);
	printd(ShowPacketInHex(*buffout, *buffoutSize).c_str(), 1);
	printd("\n", 1);
	printf("%s\n", hex2ascii(PacketInHex(*buffout, *buffoutSize)).c_str());
	printd("Packet ID (HEX) :", 1);
	printd(ShowPacketInHex(*buffout, 2).c_str(), 1);
	printd("\n", 1);
	printd("************ **********************************\n", 1);


	// *buffout is the pointer of the decrypted data starting with the packetId as a WORD
	return 1;
}

// Encrypt client packets - ONLY INTERNAL COMM
DLL_EXPORT int _pangya_client_encrypt(char *buffin, int size, char **buffout, int *buffoutSize, char key, char packetid) {
	// buffin start with the PacketId as a WORD + the data in the packet
	//"pangya_client_encrypt" must accept the decrypted packet as data starting with the Id of the packet.

	// You should allocate and encrypt buffin into *buffout here

	unsigned char  *packet_encrypt;
	packet_encrypt = (unsigned char*)malloc(1024 * 10);
	memmove(packet_encrypt, buffin, size);

	string size2;
	printd("************ pangya_client_encrypt ************\n", 2);
	printd("New Incoming Call at :", 2);
	printd(GetDateTime().c_str(), 2);
	printd("\n", 2);
	printd("******************* Input *********************\n", 2);
	printd("Size : ",2);
	size2 = to_string(size);
	printd(size2.c_str(),2);
	printd("\n", 2);
	printd("Key(HEX) : ", 2);
	printd(ShowPacketInHex(&key, 2).c_str(), 2);
	printd("\n",2);
	printd("Data (HEX) :",2);
	printd(ShowPacketInHex(buffin, size).c_str(), 2);
	printd("\n", 2);
	printf("%s\n", hex2ascii(PacketInHex(buffin, size)).c_str() );
	printd("PacketID (HEX) :", 2);
	printd(ShowPacketInHex(buffin, 2).c_str(), 2);
	printd("\n", 2);
	printd("***********************************************\n", 2);

	memmove(&packet_encrypt[5], &packet_encrypt[0],size);
	memset(packet_encrypt, 0, 5);

	int ParseKey = key << 8;

	/*
	if (key != -1)
	{
		CCompressionPacket CompressPacket;

		if (CompressPacket.Compress(&packet_encrypt[4], size) == 1)
			printf("Packet compress failed!\n");

		if (size >= CompressPacket.GetNewSize())
		{
			size = CompressPacket.GetNewSize();
			memcpy(&packet_encrypt[4], CompressPacket.GetNewBuffer(), size);
		}
		else
		{
			printf("Uncompressable packet\n");

			size = size + 3;
			memcpy(&packet_encrypt[size-3], "\x11,\x00,\x00", 3);
			packet_encrypt[size - 2] = 0x11;
			packet_encrypt[size - 1] = 0x00;
			packet_encrypt[size] = 0x00;
		}
	}
	*/
	BYTE m_nPandoraKey;
	BYTE pNum = 0x6d;//rand() % 256; 

	INT pSize = size + 1;
	size += 5;
	// add header
	
	memcpy(&packet_encrypt[0], (int*)&pNum, 1);			// packet num
	memcpy(&packet_encrypt[1], (int*)&pSize, 2);		// packet size
	if (key != -1)
	{
		m_nPandoraKey = PublicKeyTable[ParseKey + packet_encrypt[0] + 4097];
		memcpy(&packet_encrypt[3], &m_nPandoraKey, 1);	// packet key
		// encrypt packet
		Encrypt_Packet(packet_encrypt, NULL, size, ParseKey);
		
	}


	*buffoutSize = size;
	*buffout = (char*)malloc(*buffoutSize);
	memmove(*buffout, packet_encrypt, *buffoutSize);

	// work finish here
	
	printd("******************* OutPut *********************\n", 2);
	printd("Size : ", 2);
	size2 = to_string(*buffoutSize);
	printd(size2.c_str(), 2);
	printd("\n", 2);
	printf("%s\n", hex2ascii(PacketInHex(*buffout, *buffoutSize)).c_str());
	printd("Data (HEX) :", 2);
	printd(ShowPacketInHex(*buffout, *buffoutSize).c_str(), 2);
	printd("\n", 2);
	printd("************ **********************************\n", 2);

	// *buffout is the pointer of the encrypted data ready to send to Pangya
	return 1;
}


// Decrypt Pangya server packets
DLL_EXPORT int _pangya_server_decrypt(char *buffin, int size, char **buffout, int *buffoutSize, char key)
{

	// "pangya_server_decrypt" this function is not used by the server it must be present but its content can be empty
	return 1;
}

// Encrypt Pangya server packets - REAL PACKET CLIENT
DLL_EXPORT int _pangya_server_encrypt(char *buffin, int size, char **buffout, int *buffoutSize, char key)
{
	//"pangya_server_encrypt" must accept the decrypted packet as data starting with the Id of the packet.
	// buffin start with the PacketId as a WORD + the data in the packet

	unsigned char  *packet_encrypt;
	packet_encrypt = (unsigned char*)malloc(1024 * 10);
	memmove(packet_encrypt, buffin, size);
	unsigned short int tmp_byte;
	unsigned short int tmp_bl;
	string size2;

	printd("************ _pangya_server_encrypt ************\n", 4);
	printd("New Incoming Call at :", 4);
	printd(GetDateTime().c_str(), 4);
	printd("\n", 4);
	printd("******************* Input *********************\n", 4);
	printd("Size : ", 4);
	size2 = to_string(size);
	printd(size2.c_str(), 4);
	printd("\n", 4);
	printd("Key(HEX) : ", 4);
	printd(ShowPacketInHex(&key, strlen(&key)).c_str(), 4);
	printd("\n", 4);
	printd("Data (HEX) :", 4);
	printd(ShowPacketInHex(buffin, size).c_str(), 4);
	printd("\n", 4);
	printf("%s\n", hex2ascii(PacketInHex(buffin, size)).c_str());
	printd("***********************************************\n", 4);


	memmove(&packet_encrypt[4], &packet_encrypt[0], size);
	memset(packet_encrypt, 0, 4);

	int ParseKey = key << 8;
	
	if (key != -1)
	{
		CCompressionPacket CompressPacket;

		if (CompressPacket.Compress(&packet_encrypt[4], size) == 1)
			printf("packet compress failed!");

		if (CompressPacket.GetNewBuffer() && CompressPacket.GetNewSize())
		{
			size = CompressPacket.GetNewSize();
			memcpy(&packet_encrypt[4], CompressPacket.GetNewBuffer(), size);
		}
		else
		{
			printf("Failed to compress the Packet!!!");

			size = size + 3;
			memcpy(&packet_encrypt[size - 3], "\x11,\x00,\x00", 3);
			packet_encrypt[size - 2] = 0x11;
			packet_encrypt[size - 1] = 0x00;
			packet_encrypt[size] = 0x00;

		}
	}
	
	BYTE m_nPandoraKey;
	BYTE pNum = 0x6d;//rand() % 256; 


	INT pSize = size + 1;
	size += 5;
	// add header
	memcpy(&packet_encrypt[0], (int*)&pNum, 1);			// packet num
	memcpy(&packet_encrypt[1], (int*)&pSize, 2);		// packet size
	if (key != -1)
	{
		m_nPandoraKey = PublicKeyTable[key + packet_encrypt[0] + 1];
		memcpy(&packet_encrypt[3], &m_nPandoraKey, 1);	// packet key

		// encrypt packet
		Encrypt_Packet(packet_encrypt, NULL, size, ParseKey);

	}

	*buffoutSize = size;
	*buffout = (char*)malloc(*buffoutSize);
	memmove(*buffout, packet_encrypt, *buffoutSize);


	printd("******************* OutPut *********************\n", 4);
	printd("Size : ", 4);
	size2 = to_string(*buffoutSize);
	printd(size2.c_str(), 4);
	printd("\n", 4);
	printd("Data (HEX) :", 4);
	printd(ShowPacketInHex(*buffout, *buffoutSize).c_str(), 4);
	printd("\n", 4);
	printf("%s\n", hex2ascii(PacketInHex(*buffout, *buffoutSize)).c_str());
	printd("************ **********************************\n", 4);

	// *buffout is the pointer of the encrypted data ready to send to Pangya
	return 1;
}


DLL_EXPORT void _pangya_free(char **buffout) {
	// This function is used to free allocated buffout
	// It's to keep the Idea, the "class" who allocate should free the memory
	// I don't want to free the buffout from the outside because it as been allocated here
	free(*buffout);

}

// Not used for now but maybe one day
DLL_EXPORT UInt32 _pangya_deserialize(UInt32 deserialize) {
	return deserialize;
}