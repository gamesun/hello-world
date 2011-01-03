
#include "..\..\sources\Dialog\StdAfx.h"
#include "..\..\sources\common\Debug.h"
#include <windows.h>
#include <strsafe.h>


void ShowLastError( LPTSTR lpszFunction )
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
//	ExitProcess(dw); 
}


int CPU_Frequency( void ) //MHz
{
	LARGE_INTEGER CurrTicks, TicksCount;
	__int64 iStartCounter, iStopCounter;

	DWORD dwOldProcessP = GetPriorityClass(GetCurrentProcess());
	DWORD dwOldThreadP = GetThreadPriority(GetCurrentThread());

	SetPriorityClass(GetCurrentProcess() , REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread() , THREAD_PRIORITY_TIME_CRITICAL);

	QueryPerformanceFrequency(&TicksCount);
	QueryPerformanceCounter(&CurrTicks);

	TicksCount.QuadPart /= 16;
	TicksCount.QuadPart += CurrTicks.QuadPart;

	__asm rdtsc
	__asm mov DWORD PTR iStartCounter,		EAX
	__asm mov DWORD PTR (iStartCounter+4),	EDX

	while(CurrTicks.QuadPart<TicksCount.QuadPart)
	{
		QueryPerformanceCounter(&CurrTicks);
	}

	__asm rdtsc
	__asm mov DWORD PTR iStopCounter,		EAX
	__asm mov DWORD PTR (iStopCounter+4),	EDX

	SetThreadPriority(GetCurrentThread() , dwOldThreadP);
	SetPriorityClass(GetCurrentProcess() , dwOldProcessP);

	return (int)((iStopCounter-iStartCounter)/62500);
}
