/*=============================================================================================================
== PANG.DLL for Hsreina EMU
== Created by DaveDevils
=============================================================================================================*/
#define TITLE_DLL "PANG.DLL For Debug Emulator"
#define VERSION_TYPE "Debug Pangya Server"
#define	GENERAL_VER	"20170709"  // version of the dll

/*=============================================================================================================
== GENERAL CONFIGURATION 
=============================================================================================================*/
// 1 --> activate
// 0 --> OFF
#define __USE_CONSOLE  1 // if is define -> show the console
#define __USE_LOGGING  0 // if is define -> Logging to the file

#if __USE_LOGGING > 0
#define _LOG_ALLFUNC 1 // if is define -> logg all input/output of all function called by the emulator
#endif
/*=============================================================================================================
== FIX DEFINE
=============================================================================================================*/
#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>

using namespace std;
#include "logger.h"
#include "minilzo.h"
#include "CompressUncompress.h"
typedef unsigned int uint; // Using UINT more simple than unsigned int

