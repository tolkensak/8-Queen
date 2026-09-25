
#pragma once


#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */

#define ISOLATION_AWARE_ENABLED 1 // Manifest

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "targetver.h"


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>


#include <commctrl.h> /* imagelist, toolbar */
#include <commdlg.h> /* common dialogs */

#include <tolc_misc.h>
#include <tolc_draw.h>
#include <tolc_array.h>
#include <tolc_register.h>
#include <tolc_string.h>


#if _MSC_VER>1310
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif // _UNICODE
#endif
