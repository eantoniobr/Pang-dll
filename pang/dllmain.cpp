// dllmain.cpp : Defines the entry point for the DLL application.
#include "Global.h"
#include "Pang.h"

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
	#if __USE_CONSOLE > 0
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
	#endif
			string tmp;
			SetColor(1);
			printd("=================================================================\n");
			tmp  = "|                  ";
			tmp += TITLE_DLL;
			tmp += "                |\n";
			printd(tmp.c_str());
			tmp  = "|                     ";
			tmp += VERSION_TYPE;
			tmp += "                  |\n";
			SetColor(6);
			printd(tmp.c_str());
			SetColor(2);
			tmp  = "|                        ";
			tmp +="VERSION : ";
			tmp += GENERAL_VER;
			tmp += "                     |\n";
			printd(tmp.c_str());
			SetColor(1);
			printd("=================================================================\n");
			SetColor(0);
			tmp  = "Launched at ";
			tmp += GetDateTime();
			tmp += "\n";
			printd(tmp.c_str());
			printd("\n");
			break;
		}
		case DLL_PROCESS_DETACH:
			// detach from process
			break;
		case DLL_THREAD_ATTACH:
			// attach to thread
			break;

		case DLL_THREAD_DETACH:
			// detach from thread
			break;
	}
	return TRUE;
}

