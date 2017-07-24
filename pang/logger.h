#ifndef ___LOGGER_H
#define ___LOGGER_H

class Logger
{
public:
	void printd(const char * _Format, int typelog);
	void ResetColor();
	void SetColor(int color);
	void OpenLogger();
	void CloseLogger();

private:
	//Stream for file
	ofstream MainLog;
	ofstream DecryptCLog;
	ofstream EncryptCLog;
	ofstream DecryptSLog;
	ofstream EncryptSLog;
	ofstream SerialiseLog;
	int ErrorPermFile;
};

//Symplify :3
void printd(const char * _Format, int typelog = 0);
void SetColor(int color);
void OpenLogger();
void CloseLogger();
string GetDateTime();
string GetTime();
string ShowPacketInHex(unsigned char *packet_buffer, int size);
unsigned char hexval(unsigned char c);
string hex2ascii(const string& in);
string PacketInHex(unsigned char *packet_buffer, int size);

#endif