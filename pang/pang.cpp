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

int Encrypt_Packet(unsigned char *src, int len, unsigned int key, int mode_encrypt)
{
	unsigned char *data;
	data = new unsigned char[len];

	if (mode_encrypt == 1) // not used anymore
	{
		memcpy(&data[0], &src[0], len);

		for (int i = 8; i < len; i++)
		{
			data[i] = data[i] ^ src[i - 4];
		}
		data[3] = data[3] ^ PublicKeyTable[key + data[0]];
		data[4] = data[4] ^ PublicKeyTable[key + data[0]];
		memcpy(&src[0], &data[0], len);
	}
	else if (mode_encrypt == 0)
	{	
		memcpy(&data[0], &src[0], len);	
		data[3] = data[3] ^ PublicKeyTable[key + data[0]];
		data[4] = data[4] ^ PublicKeyTable[key + data[0]];
		for (int i = 8; i < len; i++)
		{
			data[i] = data[i] ^ src[i - 4];
		}
		memcpy(&src[0], &data[0], len);

	}

	delete[](data);

	return 1;
}

int Decrypt_Packet(unsigned char *src, int len, unsigned int key, int mode_encrypt)
{
	//Some Edit By HSREINA

		unsigned char *data;

		if (mode_encrypt == 1)// not used anymore
		{		
			src[4] = PublicKeyTable[src[0] + key] ^ src[4];
			// Delete Header part
			len -= 4;
			data = new unsigned char[len];
			memcpy(&data[0], &src[4], len);

			// Decrypt Data packet
			for (unsigned long i = 4; i < (unsigned)len; i++)
			{
				data[i] = data[i] ^ data[i - 4];
			}

			//Delete One byte
			len -= 1;
			memmove(&src[0], &data[1], len);
		}
		else if (mode_encrypt == 0)
		{
			data = new unsigned char[len];
			memcpy(&data[0], &src[0], len);

			data[4] = data[4] ^ PublicKeyTable[key + data[0]];
			unsigned long  i;

			for (unsigned long i = 8; i < (unsigned)len; i++)
			{
				data[i] = data[i] ^ data[i - 4];
			}
			memcpy(&src[0], &data[0], len);

		}
		delete[](data);
	return 1;
}


// Decrypt pangya client packets
DLL_EXPORT int _pangya_client_decrypt(unsigned char *buffin, int size, unsigned char **buffout, int *buffoutSize, unsigned char key)
{
	// buffin is the pointer of Data received by the Pangya Client.
	// That mean, when you have a packet from the client, you must give it to this function.
	// This function should only know how to decrypt a single packet

	// "pangya_client_decrypt" must accept the full packet send by the client as data and must return the decrypted packet starting with the Id of the packet


	unsigned char *packet_decrypt;
	packet_decrypt = new unsigned char[size * 2];
	memcpy(packet_decrypt, buffin, size);


	string size2;
	string InputLog = "************ pangya_client_decrypt ************\n";
	InputLog += "New Incoming Call at :" + GetDateTime() + "\n";
	InputLog += "******************* Input *********************\n";
	size2 = to_string(size);
	InputLog += "Size : " + size2 + "\n";
	InputLog += "Key (HEX) : " + ShowPacketInHex(&key, 1) + "\n";
	InputLog += hex2ascii(PacketInHex(packet_decrypt, size)) + "\n";
	InputLog += "Data (HEX) :" + ShowPacketInHex(packet_decrypt, size) + "\n";
	InputLog += "PacketID (HEX) :" + ShowPacketInHex(packet_decrypt, 2) + "\n";
	InputLog += "***********************************************\n";
	printd(InputLog.c_str(), 1);


	if (size > 8)
	{

			int ParseKey = key << 8;
			Decrypt_Packet(packet_decrypt, size, ParseKey, 0);
			size -= 3;
			memcpy(&packet_decrypt[0], &packet_decrypt[3], size);

			InputLog = hex2ascii(PacketInHex(packet_decrypt, size)) + "\n";
			InputLog += "Data (HEX) :" + ShowPacketInHex(packet_decrypt, size) + "\n";
			printd(InputLog.c_str(), 1);

			if (packet_decrypt[size - 3] == 0x11 && packet_decrypt[size - 2] == 0x00 && packet_decrypt[size - 1] == 0x00)
			{
				int NewSize = size - 5;
				memcpy(&packet_decrypt[0], &packet_decrypt[5], NewSize);
				CCompressionPacket * CompressBuffer = new CCompressionPacket;
				uint OrgSize = CompressBuffer->Decompress(packet_decrypt, NewSize);
				packet_decrypt = new unsigned char[OrgSize];
				packet_decrypt = CompressBuffer->GetNewBuffer();
				size = OrgSize;
				delete(CompressBuffer);
			}
			else
			{
				size -= 2;
				memmove(&packet_decrypt[0], &packet_decrypt[2], size);
			}



	}

	*buffoutSize = size;
	*buffout =  new unsigned char[*buffoutSize];
	memcpy(*buffout, &packet_decrypt[0], *buffoutSize);
	delete[](packet_decrypt);


	// work finish here

	string OutputLog = "******************* OutPut *********************\n";
	size2 = to_string(size);
	OutputLog += "Size : " + size2 + "\n";
	OutputLog += hex2ascii(PacketInHex(*buffout, size)) + "\n";
	OutputLog += "Data (HEX) :" + ShowPacketInHex(*buffout, size) + "\n";
	OutputLog += "***********************************************\n";
	printd(OutputLog.c_str(), 1);

	// *buffout is the pointer of the decrypted data starting with the packetId as a WORD
	return 1;

}

// Encrypt client packets - ONLY INTERNAL COMM
DLL_EXPORT int _pangya_client_encrypt(unsigned char *buffin, int size, unsigned char **buffout, int *buffoutSize, unsigned char key) {
	// buffin start with the PacketId as a WORD + the data in the packet
	//"pangya_client_encrypt" must accept the decrypted packet as data starting with the Id of the packet.

	// You should allocate and encrypt buffin into *buffout here
		unsigned char  *packet_encrypt;
		packet_encrypt = new unsigned char[size+4];
		memcpy(packet_encrypt, buffin, size);

		string size2;

		string InputLog = "************ pangya_client_encrypt ************\n";
		InputLog += "New Incoming Call at :" + GetDateTime() + "\n";
		InputLog += "******************* Input *********************\n";
		size2 = to_string(size);
		InputLog += "Size : " + size2 + "\n";
		InputLog += "Key (HEX) : " + ShowPacketInHex(&key, 1) + "\n";
		InputLog += hex2ascii(PacketInHex(packet_encrypt, size)) + "\n";
		InputLog += "Data (HEX) :" + ShowPacketInHex(packet_encrypt, size) + "\n";
		InputLog += "PacketID (HEX) :" + ShowPacketInHex(packet_encrypt, 2) + "\n";
		InputLog += "***********************************************\n";
		printd(InputLog.c_str(), 2);

		BOOL COMPRESS_PACKET = true;

		int ParseKey = key << 8;
		if (size > 5)
		{
			memmove(&packet_encrypt[4], &packet_encrypt[0], size);
			memset(packet_encrypt, 0, 4);

			if (ParseKey != -1 && COMPRESS_PACKET == TRUE)
			{

				CCompressionPacket * CompressPacket = new CCompressionPacket;
				if (CompressPacket->Compress(&packet_encrypt[4], size) == 1)
					printf("packet compress failed!");

				if (CompressPacket->GetNewBuffer() && CompressPacket->GetNewSize())
				{
					size = CompressPacket->GetNewSize()+1;
					delete[](packet_encrypt);
					packet_encrypt = new unsigned char[size+4];
					memcpy(&packet_encrypt[4], CompressPacket->GetNewBuffer(), size);

					InputLog = hex2ascii(PacketInHex(packet_encrypt, size)) + "\n";
					InputLog += "Data (HEX) :" + ShowPacketInHex(packet_encrypt, size) + "\n";
					printd(InputLog.c_str(), 2);
				}
				else
				{
					printf("Failed to compress the Packet!!!");
				}
				delete(CompressPacket);

			}

			srand((unsigned)time(NULL));
			BYTE pNum = rand() % 256;
			INT pSize = size - 1;
			size += 3;
			
			packet_encrypt[0] = pNum;

			memcpy(&packet_encrypt[1], &pSize, 2);

			BYTE m_nPandoraKey = PrivateKeyTable[ParseKey + pNum];
			packet_encrypt[3] = m_nPandoraKey;	

			
			if (ParseKey != -1)
			{
				Encrypt_Packet(packet_encrypt, size, ParseKey, 0);
			}

		}
			*buffoutSize = size;
			*buffout =  new unsigned char[*buffoutSize];
			memcpy(*buffout, &packet_encrypt[0], *buffoutSize);
			delete[](packet_encrypt);

		// work finish here

		string OutputLog = "******************* OutPut *********************\n";
		size2 = to_string(size);
		OutputLog += "Size : " + size2 + "\n";
		OutputLog += hex2ascii(PacketInHex(*buffout, size)) + "\n";
		OutputLog += "Data (HEX) :" + ShowPacketInHex(*buffout, size) + "\n";
		OutputLog += "***********************************************\n";
		printd(OutputLog.c_str(), 2);

		// *buffout is the pointer of the encrypted data ready to send to Pangya
		return 1;
}

// Decrypt Pangya server packets
DLL_EXPORT int _pangya_server_decrypt(unsigned char *buffin, int size, unsigned char **buffout, int *buffoutSize, unsigned char key)
{
	// buffin is the pointer of Data received by the Pangya Client.
	// That mean, when you have a packet from the client, you must give it to this function.
	// This function should only know how to decrypt a single packet
	// "pangya_client_decrypt" must accept the full packet send by the client as data and must return the decrypted packet starting with the Id of the packet

	//memmove(*buffout, buffin, size);


	// "pangya_server_decrypt" this function is not used by the server it must be present but its content can be empty

	unsigned char *packet_decrypt;
	packet_decrypt = new unsigned char[size * 2];
	memcpy(packet_decrypt, buffin, size);


		string size2;
		string InputLog = "************ pangya_server_decrypt ************\n";
		InputLog += "New Incoming Call at :" + GetDateTime() + "\n";
		InputLog += "******************* Input *********************\n";
		size2 = to_string(size);
		InputLog += "Size : " + size2 + "\n";
		InputLog += "Key (HEX) : " + ShowPacketInHex(&key, 1) + "\n";
		InputLog += hex2ascii(PacketInHex(packet_decrypt, size)) + "\n";
		InputLog += "Data (HEX) :" + ShowPacketInHex(packet_decrypt, size) + "\n";
		InputLog += "PacketID (HEX) :" + ShowPacketInHex(packet_decrypt, 2) + "\n";
		InputLog += "***********************************************\n";
		printd(InputLog.c_str(), 3);

			if (size > 8)
			{

				int ParseKey = key << 8;
				Decrypt_Packet(packet_decrypt, size, ParseKey, 0);
				size -= 3;
				memcpy(&packet_decrypt[0], &packet_decrypt[3], size);

				InputLog = hex2ascii(PacketInHex(packet_decrypt, size)) + "\n";
				InputLog += "Data (HEX) :" + ShowPacketInHex(packet_decrypt, size) + "\n";
				printd(InputLog.c_str(), 1);

				if (packet_decrypt[size - 3] == 0x11 && packet_decrypt[size - 2] == 0x00 && packet_decrypt[size - 1] == 0x00)
				{
					int NewSize = size - 5; 
					memcpy(&packet_decrypt[0], &packet_decrypt[5], NewSize);
					CCompressionPacket * CompressBuffer = new CCompressionPacket;
					uint OrgSize = CompressBuffer->Decompress(packet_decrypt, NewSize);
					packet_decrypt = new unsigned char[OrgSize];
					packet_decrypt = CompressBuffer->GetNewBuffer();
					size = OrgSize;
					delete(CompressBuffer);
				}
				else
				{
					size -= 2;
					memmove(&packet_decrypt[0], &packet_decrypt[2], size);
				}

			}

			*buffoutSize = size;
			*buffout = new unsigned char[*buffoutSize];
			memcpy(*buffout, &packet_decrypt[0], *buffoutSize);
			delete[](packet_decrypt);

			// work finish here

			string OutputLog = "******************* OutPut *********************\n";
			size2 = to_string(size);
			OutputLog += "Size : " + size2 + "\n";
			OutputLog += hex2ascii(PacketInHex(*buffout, size)) + "\n";
			OutputLog += "Data (HEX) :" + ShowPacketInHex(*buffout, size) + "\n";
			OutputLog += "***********************************************\n";
			printd(OutputLog.c_str(), 3);

		return 1;

}

// Encrypt Pangya server packets - REAL PACKET CLIENT
DLL_EXPORT int _pangya_server_encrypt(unsigned char *buffin, int size, unsigned char **buffout, int *buffoutSize, unsigned char key)
{
	//"pangya_server_encrypt" must accept the decrypted packet as data starting with the Id of the packet.
	// buffin start with the PacketId as a WORD + the data in the packet

		unsigned char  *packet_encrypt;
		packet_encrypt = new unsigned char[size+5];
		memcpy(packet_encrypt, buffin, size);

		string size2;
		
		string InputLog = "************ _pangya_server_encrypt ************\n";
		InputLog += "New Incoming Call at :" + GetDateTime() + "\n";
		InputLog += "******************* Input *********************\n";
		size2 = to_string(size);
		InputLog += "Size : " + size2 + "\n";
		InputLog += "Key (HEX) : " + ShowPacketInHex(&key, 1) + "\n";
		InputLog += hex2ascii(PacketInHex(packet_encrypt, size)) + "\n";
		InputLog += "Data (HEX) :" + ShowPacketInHex(packet_encrypt, size) + "\n";
		InputLog += "PacketID (HEX) :" + ShowPacketInHex(packet_encrypt, 2) + "\n";
		InputLog += "***********************************************\n";
		printd(InputLog.c_str(), 4);

		int ParseKey = key << 8;
		if (size > 5)
		{
			memmove(&packet_encrypt[4], &packet_encrypt[0], size);
			memset(packet_encrypt, 0, 4);

			if (ParseKey != -1)
			{

				CCompressionPacket * CompressPacket = new CCompressionPacket;
				if (CompressPacket->Compress(&packet_encrypt[4], size) == 1)
					printf("packet compress failed!");

				if (CompressPacket->GetNewBuffer() && CompressPacket->GetNewSize())
				{
					size = CompressPacket->GetNewSize() + 1 ;
					delete[](packet_encrypt);
					packet_encrypt = new unsigned char[size + 4];
					memcpy(&packet_encrypt[4], CompressPacket->GetNewBuffer(), size);

					InputLog = hex2ascii(PacketInHex(packet_encrypt, size)) + "\n";
					InputLog += "Data (HEX) :" + ShowPacketInHex(packet_encrypt, size) + "\n";
					printd(InputLog.c_str(), 2);
				}
				else
				{
					printf("Failed to compress the Packet!!!");
				}
				delete(CompressPacket);
			}

			srand((unsigned)time(NULL));
			BYTE pNum = rand() % 256;
			INT pSize = size - 1;
			size += 3;

			packet_encrypt[0] = pNum;			
			memcpy(&packet_encrypt[1], &pSize, 2);		

			BYTE m_nPandoraKey = PrivateKeyTable[ParseKey + pNum];
			packet_encrypt[3] = m_nPandoraKey;	

			if (ParseKey != -1)
			{
				Encrypt_Packet(packet_encrypt, size, ParseKey, 0);
			}

		}

		*buffoutSize = size;
		*buffout =  new unsigned char[*buffoutSize];
		memcpy(*buffout, &packet_encrypt[0], *buffoutSize);
		delete[](packet_encrypt);


		// work finish here

		string OutputLog = "******************* OutPut *********************\n";
		size2 = to_string(size);
		OutputLog += "Size : " + size2 + "\n";
		OutputLog += hex2ascii(PacketInHex(*buffout, size)) + "\n";
		OutputLog += "Data (HEX) :" + ShowPacketInHex(*buffout, size) + "\n";
		OutputLog += "***********************************************\n";
		printd(OutputLog.c_str(), 4);

		// *buffout is the pointer of the encrypted data ready to send to Pangya
		return 1;
}


DLL_EXPORT void _pangya_free(unsigned char **buffout) {
	// This function is used to free allocated buffout
	// It's to keep the Idea, the "class" who allocate should free the memory
	// I don't want to free the buffout from the outside because it as been allocated here
	delete(*buffout);

}

// Not used for now but maybe one day
DLL_EXPORT UInt32 _pangya_deserialize(UInt32 deserialize) {
	return deserialize;
}