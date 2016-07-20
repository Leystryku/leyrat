#pragma once


#pragma comment(linker, "/NODEFAULTLIB")
#pragma comment(linker, "/INCREMENTAL:NO")
#pragma comment(linker, "/HEAP:0")
#pragma comment(linker, "/STACK:0")

#pragma optimize("gsy", on)

#define _NO_CRT_STDIO_INLINE
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define _CRT_SECURE_NO_WARNINGS


extern "C" void mainCRTStartup(void);