// System functions (Win32 version!)
#include <windows.h>
#include <stdio.h>

#include "WolfDef.h"

typedef enum
{
	os_unknown,
	os_Win95,
	os_WinNT,
	os_Win32s
} os_t;

char MMXsupport=0;
char XMMsupport=0;
os_t OSPlatform;

void Sys_OSinfo(void)
{
	OSVERSIONINFO info;
	char *osname;

	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&info);

	switch(info.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s:
		OSPlatform=os_Win32s;
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		OSPlatform=os_Win95;
		if(info.dwMinorVersion<10)
		{// Windows 95
			if(info.dwBuildNumber<1111)
				osname="Windows 95";
			else
				osname="Windows 95 OSR2";
		}
		else if(info.dwMinorVersion<90)
		{// Windows 98

			if(info.dwBuildNumber<1111)
				osname="Windows 98";
			else
				osname="Windows 98 SE";
		}
		else
		{// Windows Me
			osname="Windows Me";
		}
	break;
	case VER_PLATFORM_WIN32_NT:
		OSPlatform=os_WinNT;
		if(info.dwMajorVersion>=5)
		{ // NT based Windows (2000, XP)
			if(info.dwMinorVersion>=1)
				osname="Windows XP";
			else
				osname="Windows 2000";
		}
		else
		{ // Windows NT
			osname="Windows NT";
		}
		break;
	default:
		OSPlatform=os_unknown;
		osname="Unknown OS";
		break;
	}

	Con_Printf("-> OS: %s %u.%u.%u %s\n",
			osname,
			info.dwMajorVersion, info.dwMinorVersion,
			info.dwBuildNumber & (OSPlatform == os_Win95 ? 0xffff : 0xfffffff),
			info.szCSDVersion);

	if(OSPlatform == os_unknown)
	{
		Con_Printf("-> (Assuming Windows 95)\n");
		OSPlatform=os_Win95;
	}
}

#pragma warning(disable:4035)
__inline __int64 GetCycleNumber(void) {__asm RDTSC}
#pragma warning(default:4035)

int Sys_CPUspeed(void)
{
	LARGE_INTEGER HWst, HW, HWf;
	__int64 CPU;

	if( !QueryPerformanceFrequency(&HWf) ) return 0;

	QueryPerformanceCounter(&HWst);
	CPU=GetCycleNumber();
	__asm
	{
		mov ebx, 5000000
	CPUwait:
		dec ebx
		jnz CPUwait
	}
	QueryPerformanceCounter(&HW);
	CPU=GetCycleNumber()-CPU;
	HW.QuadPart-=HWst.QuadPart;
	if(HW.QuadPart==0) return 0; // do not repeat borland errors!
	return (int)( CPU*HWf.QuadPart/HW.QuadPart/1000000 );
}

void Sys_CPUinfo(void)
{
	unsigned long i0, i3;
	char vendor[16];
	int speed;

	_asm
	{
		mov		eax, 0
		cpuid
		mov		dword ptr [vendor], ebx
		mov   dword ptr [vendor+4], edx
		mov   dword ptr [vendor+8], ecx
		mov   dword ptr [vendor+12], 0

		mov		eax, 1
		cpuid
		mov		i0, eax
		mov   i3, edx
	}

	Con_Printf("-> CPU speed: ");
	speed=Sys_CPUspeed();
	if(speed>0)
		Con_Printf("%d MHz\n", speed);
	else
		Con_Printf("unable to detect\n");
	Con_Printf("-> Family %d, Model %d, Stepping %d\n", (i0>>8)&0x0f, (i0>>4)&0x0f, i0&0x0f);
	Con_Printf("-> Extended info: 0x%08X\n", i3);
	Con_Printf("-> FPU %sDetected\n", i3&(1<<0) ? "" : "NOT ");
	Con_Printf("-> MMX %sDetected\n", i3&(1<<23) ? MMXsupport=1, "" : "NOT ");
	Con_Printf("-> XMM(PIII) %sDetected\n", i3&(1<<25) ? XMMsupport=1, "" : "NOT ");
	Con_Printf("-> CPU Id: %s\n", vendor);
}

void Sys_MEMinfo(void)
{
	MEMORYSTATUS w32mem;

	GlobalMemoryStatus(&w32mem);
	Con_Printf("-> Physical memory: %dMb (%dMb free)\n", w32mem.dwTotalPhys/1024/1024, w32mem.dwAvailPhys/1024/1024);
	Con_Printf("-> Total memory in use: %d%%\n", w32mem.dwMemoryLoad);
}

void Sys_Init(void)
{
	SYSTEMTIME time;
#ifdef _DEBUG
	char dbg[]="DEBUG";
#else
	char dbg[]="RELEASE";
#endif

	GetLocalTime(&time);
	Con_Printf("\n******************************\nWolfGL Version "VERSION" (%d-%s)\nBuilt on "__DATE__", "__TIME__"\n", BUILD, dbg);
	Con_Printf("Ran on    %02d/%02d/%02d, %02d:%02d:%02d\n\n", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond);

	Con_Printf("Sys_Init: CPU info\n");
	Sys_CPUinfo();
	Con_Printf("Sys_Init: Memory info\n");
	Sys_MEMinfo();
	Con_Printf("Sys_Init: OS info\n");
	Sys_OSinfo();
}
