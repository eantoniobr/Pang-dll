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

int Encrypt_Packet(unsigned char *src, int len, unsigned int key)
{
	unsigned char *data;
	data = (unsigned char*)malloc(len);
	memcpy(&data[0], &src[0], len);
	
	for (int i = 8; i < len; i++)
	{
		data[i] = data[i] ^ src[i - 4];
	}
	data[3] = data[3] ^ PublicKeyTable[key + data[0]];
	data[4] = data[4] ^ PublicKeyTable[key + data[0]];
	memmove(&src[0], &data[0], len);
	free(data);
	return 1;
}

int Decrypt_Packet(unsigned char *src,  int len, unsigned int key)
{
	//Some Edit By HSREINA

	if (len > 4)
	{
		// Decrypt the 1st byte
		src[4] = PublicKeyTable[src[0] + key] ^ src[4];

		// Delete Header part
		len -= 4;
		unsigned char *data;
		data = (unsigned char*)malloc(len);
		memcpy(&data[0], &src[4], len);

		// Decrypt Data packet
		for (unsigned long i = 4; i < len; i++)
		{
			data[i] = data[i] ^ data[i - 4];
		}

		//Delete One byte
		len -= 1;
		memmove(&src[0], &data[1], len);
		free(data);
	}
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
	printd(ShowPacketInHex(&key, 1).c_str(), 1);
	printd("\n", 1);
	printf("%s\n", hex2ascii(PacketInHex(buffin, size)).c_str());
	printd("Data (HEX) :", 1);
	printd(ShowPacketInHex(buffin, size).c_str(), 1);
	printd("\n", 1);
	printd("***********************************************\n", 1);
	
	int ParseKey =  key << 8 ;
	if (size > 4)
	{
		//Send to Decrypt Part
		Decrypt_Packet(packet_decrypt, size, ParseKey);
		/*


		// UNCOMPRESS Part
		int NewSize = size;
		unsigned char *tempPacket;
		tempPacket = (unsigned char*)malloc(NewSize);
		memcpy(tempPacket, (char*)packet_decrypt[0], NewSize);
		CCompressionPacket CompressionPacket;
		int NewSize2 = CompressionPacket.Decompress(tempPacket, NewSize);
		unsigned char *tempPacket2 = CompressionPacket.GetNewBuffer();
		memcpy(&packet_decrypt[0], &tempPacket2[0], NewSize2);
		size = NewSize2;
		*/



		*buffoutSize = size - 5;
		*buffout = (char*)malloc(*buffoutSize);
		memmove(*buffout, &packet_decrypt[0], *buffoutSize);
	}
	else
	{
		*buffoutSize = size;
		*buffout = (char*)malloc(*buffoutSize);
		memmove(*buffout, &packet_decrypt[0], *buffoutSize);
	}

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

	if (size > 5)
	{

		memmove(&packet_encrypt[5], &packet_encrypt[0], size);
		memset(packet_encrypt, 0x00, 4);

		int ParseKey = key << 8;

		
		/*
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
		}
		
		*/
		BYTE m_nMagicKey;
		BYTE pNum = rand() % 256;//0x6d
		int PSize = size + 1;
		size += 5;


		// add header
		memcpy(&packet_encrypt[0], &pNum, 1);			// packet RandomNumber
		memcpy(&packet_encrypt[1], &PSize, 2);			// packet size

		m_nMagicKey = PublicKeyTable[ParseKey + packet_encrypt[0] + 4097]; // 0xcc;
		memcpy(&packet_encrypt[3], &m_nMagicKey, 1);	// packet Magickey

		Encrypt_Packet(packet_encrypt, size, ParseKey); // encrypt packet

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
	printd(ShowPacketInHex(&key, 1).c_str(), 4);
	printd("\n", 4);
	printd("Data (HEX) :", 4);
	printd(ShowPacketInHex(buffin, size).c_str(), 4);
	printd("\n", 4);
	printf("%s\n", hex2ascii(PacketInHex(buffin, size)).c_str());
	printd("***********************************************\n", 4);


		memmove(&packet_encrypt[5], &packet_encrypt[0], size);
		memset(packet_encrypt, 0, 5);

		int ParseKey = key << 8;
		/*
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
			}
		}
		*/
		BYTE m_nMagicKey;
		BYTE pNum = rand() % 256;//0x6d
		int PSize = size + 1;
		size += 4;


		// add header
		memcpy(&packet_encrypt[0], &pNum, 1);			// packet RandomNumber
		memcpy(&packet_encrypt[1], &PSize, 2);			// packet size

		m_nMagicKey = PublicKeyTable[ParseKey + packet_encrypt[0] + 4097]; // 0xcc;
		memcpy(&packet_encrypt[3], &m_nMagicKey, 1);	// packet Magickey

		Encrypt_Packet(packet_encrypt, size, ParseKey); // encrypt packet


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