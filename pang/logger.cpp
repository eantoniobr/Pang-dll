// logger part *-*
#include "Global.h"

//First part --> external Call for all part

void printd(const char * _Format, int typelog)
{
	Logger Call;
	Call.printd(_Format,typelog);
}

void SetColor(int color)
{
	Logger Call;
	Call.SetColor(color);
}
void OpenLogger()
{
	Logger Call;
	Call.OpenLogger();
}

void CloseLogger()
{
	Logger Call;
	Call.CloseLogger();
}

string ShowPacketInHex(char *packet_buffer, int size)
{
	char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		string returndata = "";
		for (int i = 0; i < size; ++i) 
		{
			const char ch = packet_buffer[i];
			returndata += " 0x";
			returndata.append(&hex[(ch & 0xF0) >> 4], 1);
			returndata.append(&hex[ch & 0xF], 1);
		}
	return returndata;
}

string PacketInHex(char *packet_buffer, int size)
{
	char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	string returndata = "";
	for (int i = 0; i < size; ++i)
	{
		const char ch = packet_buffer[i];
		returndata.append(&hex[(ch & 0xF0) >> 4], 1);
		returndata.append(&hex[ch & 0xF], 1);
	}
	return returndata;
}

unsigned char hexval(unsigned char c)
{
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'f')
		return c - 'a' + 10;
	else if ('A' <= c && c <= 'F')
		return c - 'A' + 10;

	return '0';
}

string hex2ascii(const string& in )
{
	string out;
	out = "Data (STRING) :";
	for (string::const_iterator p = in.begin(); p != in.end(); p++)
	{
		unsigned char c = hexval(*p);
		p++;
		if (p == in.end()) break; 
		c = (c << 4) + hexval(*p); 

		if (c == NULL)
			c = ' ';

		out += std::string(1, c);
	}

	return out;
}

string GetDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       time[50];
	localtime_s(&tstruct, &now);
	strftime(time, sizeof(time), "%d-%m-%Y %X", &tstruct);
	return time;
}

string GetTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       time[20];
	localtime_s(&tstruct, &now);
	strftime(time, sizeof(time), "%X", &tstruct);
	return time;
}

// Second part --> real function :D

void Logger::printd(const char * _Format, int typelog)
{

	string Format = _Format;

#if __USE_LOGGING > 0
	OpenLogger();
	if (typelog == 0)
	{
		if (MainLog.is_open())
		{
			MainLog << Format;
		}
		else if(ErrorPermFile == 0)
		{
			ErrorPermFile = 1;
			SetColor(12);
			printf("Erreur de permission pour écrire Main-PangDLL.log!\n");
			SetColor(0);
		}
	}
#if _LOG_ALLFUNC > 0
	if (typelog == 1)
	{
		if (DecryptCLog.is_open())
		{
			DecryptCLog << Format ;
		}
		else if (ErrorPermFile == 0)
		{
			ErrorPermFile = 1;
			SetColor(12);
			printf("Erreur de permission pour écrire client-decrypt-PangDLL.log !\n");
			SetColor(0);
		}
	}
	if (typelog == 2)
	{
		if (EncryptCLog.is_open())
		{
			EncryptCLog << Format;
		}
		else if (ErrorPermFile == 0)
		{
			ErrorPermFile = 1;
			SetColor(12);
			printf("Erreur de permission pour écrire client-encrypt-PangDLL.log !\n");
			SetColor(0);
		}
	}
	if (typelog == 3)
	{
		if (DecryptSLog.is_open())
		{
			DecryptSLog << Format;
		}
		else if (ErrorPermFile == 0)
		{
			ErrorPermFile = 1;
			SetColor(12);
			printf("Erreur de permission pour écrire server-decrypt-PangDLL.log !\n");
			SetColor(0);
		}
	}
	if (typelog == 4)
	{
		if (EncryptSLog.is_open())
		{
			EncryptSLog  << Format;
		}
		else if (ErrorPermFile == 0)
		{
			ErrorPermFile = 1;
			SetColor(12);
			printf("Erreur de permission pour écrire server-encrypt-PangDLL.log !\n");
			SetColor(0);
		}
	}
#endif
	CloseLogger();
#endif

#if __USE_CONSOLE > 0
	printf(_Format); // print to the console
#endif
}

void Logger::OpenLogger()
{
#if __USE_LOGGING > 0
	ErrorPermFile = 0;
	MainLog.open("Main-PangDLL.log", ios::out | ios::app);
#if _LOG_ALLFUNC > 0
	DecryptCLog.open("client-decrypt-PangDLL.log", ios::out | ios::app);
	EncryptCLog.open("client-encrypt-PangDLL.log", ios::out | ios::app);
	DecryptSLog.open("server-decrypt-PangDLL.log", ios::out | ios::app);
	EncryptSLog.open("server-encrypt-PangDLL.log", ios::out | ios::app);
#endif

#endif
}

void Logger::CloseLogger()
{
#if __USE_LOGGING > 0
	MainLog.close();
#if _LOG_ALLFUNC > 0
	DecryptCLog.close();
	EncryptCLog.close();
	DecryptSLog.close();
	EncryptSLog.close();
#endif 

#endif
}

void Logger::SetColor(int color)
{
#ifdef _WIN32
	switch (color)
	{
	case 0:    // White on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 1:    // Red on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case 2:    // Green on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case 3:    // Yellow on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case 4:    // Blue on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		break;
	case 5:    // Magenta on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case 6:    // Cyan on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 7:    // Black on Gray
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_INTENSITY);
		break;
	case 8:    // Black on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case 9:    // Red on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED);
		break;
	case 10:    // Green on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN);
		break;
	case 11:    // Yellow on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case 12:    // Blue on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE);
		break;
	case 13:    // Magenta on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case 14:    // Cyan on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 15:    // White on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:    // White on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
#elif LINUX
#endif
}

void Logger::ResetColor()
{
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#elif LINUX
#endif
}