/****************************************************************************/
/*	Virtual Oscilloscopes Upper Computer Application						*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	File Name	:	RecvThread.cpp											*/
/*	Coder		:	sun.yt													*/
/*	Purpose		:															*/
/*																			*/
/****************************************************************************/

#pragma once

#include "RecvThread.h"
#include "..\..\sources\common\Global.h"

#define DEBUG_RECV_TIME_COST 1

int CPU_Frequency( void );

//-----------------------------------------------------------------------------
// Name: RecvThreadFunc
// Desc: 
//-----------------------------------------------------------------------------
UINT RecvThreadFunc( LPVOID lpParam )
{
//	TRACE( "Enter RecvThreadFunc()\n" );
	int				nOldPos;
//	bool			bRecvDigitHigh;
//	SHORT			nTmpH;
	char			nTmpL;
	long			i;
	int				len;
//	char			szRxData[1024];
//	VARIANT			variant_inp;
	COleSafeArray	safearray_inp;
	
	nOldPos			= 0;
//	bRecvDigitHigh	= true;
//	nTmpH			= 0;
	nTmpL			= 0;
	i				= 0;
	len				= 0;

	int			*pm_nXCnt				= &g_pMainThis->m_nXCnt;
	int			*pm_nSampPerFrame		= &g_pMainThis->m_nSampPerFrame;
	int			*pm_nNumOfDiv			= &g_pMainThis->m_nNumOfDiv;
	long		*pm_nQueueRearOffset	= &g_pMainThis->m_nQueueRearOffset;
	bool		*pm_bRecvThreadRunning	= &g_pMainThis->m_bRecvThreadRunning;
	CScrollBar	*pm_ctrlSbDrawFrame		= &( g_pMainThis->m_ctrlSbDrawFrame );

	while ( 1 )
	{
		WaitForSingleObject( g_eventCommRecv.m_hObject, INFINITE );
		if ( false == *pm_bRecvThreadRunning )
		{
			break;
		}
//		TRACE( "    Ask->Recv\n" );
		g_ccsRWData.Lock();
//		TRACE( "    ->Recv\n" );

#if DEBUG_RECV_TIME_COST
//		start = clock();
		LARGE_INTEGER nFreq;
		LARGE_INTEGER nLastTime1;
		LARGE_INTEGER nLastTime2;
		
		QueryPerformanceFrequency( &nFreq );
//		TRACE("Frequency:%I64d\r\n", nFreq );

		QueryPerformanceCounter( &nLastTime1 );
		long   HighStart,LowStart,HighEnd,LowEnd; 
		long   numhigh,numlow;

		__asm 
		{ 
			RDTSC   
			mov		HighStart,	edx 
			mov		LowStart,	eax	
		}
#endif /* RECV_THREAD_TIME_COST */
/*	
		variant_inp = g_pMainThis->m_mscom.GetInput();
		
		safearray_inp = variant_inp;
		len = safearray_inp.GetOneDimSize();
		for ( i = 0; i < len; i++ )
		{
			safearray_inp.GetElement( &i, &szRxData[i] );
//			szRxData[i] &= 0xff;
			
			nTmpL = szRxData[0] & 0xff;
//			TRACE( "L %02x\n", nTmpL );
			INSQueue( g_psQueue, nTmpL );
			(*pm_nXCnt)++;
//			TRACE( "0x%08x ", szRxData[i] );
		}
		
//		szRxData[i] = '\0';
//		TRACE( "%s \n", szRxData );
//		TRACE( "\n" );
		


/**/

/////////////////////////// TEST ///////////////////////////
		
		static int m = 0;

		INSQueue( g_psQueue, (QUDA)g_nTest[m] );
		m = ( m + 1 ) % 200;
		(*pm_nXCnt)++;
//		Sleep( 1 );
/**/
////////////////////////////////////////////////////////////

		if ( *pm_nXCnt >= *pm_nSampPerFrame )
		{
			*pm_nXCnt %= *pm_nSampPerFrame;
		}

		*pm_nNumOfDiv = ( g_psQueue->num - *pm_nXCnt ) / 
						( *pm_nSampPerFrame / 12 );
		
//		pm_ctrlSbDrawFrame->SetScrollRange( 0, *pm_nNumOfDiv );
//		PostMessage( GetDlgItem( g_pMainThis->m_hWnd, IDC_SBDRAWFRAME ), 
//					SBM_SETRANGE, FALSE, *pm_nNumOfDiv );
		
//		g_pMainThis->m_ctrlSbDrawFrame.SetScrollRange( 0, *pm_nNumOfDiv );
		
		nOldPos = pm_ctrlSbDrawFrame->GetScrollPos();
		
		pm_ctrlSbDrawFrame->SetScrollPos( *pm_nNumOfDiv, FALSE );
		
		if ( nOldPos != *pm_nNumOfDiv )
		{
			*pm_nQueueRearOffset = 0;
			g_pMainThis->UpdateFrame( CLEAR );
		}
		else
		{
			g_pMainThis->UpdateFrame( BASIC );
		}

#if DEBUG_RECV_TIME_COST

		__asm
		{
			RDTSC
			mov		HighEnd,	edx 
			mov		LowEnd,		eax 
			//获取两次计数器值得差 
			sub		eax,		LowStart 
			cmp		eax,		0               
			jg		L1 
			neg		eax   
			jmp		L2
L1:			mov		numlow,		eax 
L2:			sbb		edx,		HighStart 
			mov		numhigh,	edx 
		} 
//		__int64 timer = (numhigh<<32) + numlow;
		
		TRACE( "num:%d,%d\r\n", numhigh, numlow );

		QueryPerformanceCounter( &nLastTime2 );
//		finish = clock();

//		TRACE( "LastTime1:%I64d\r\n", nLastTime1 );
//		TRACE( "LastTime2:%I64d\r\n", nLastTime2 );

		double fInterval = nLastTime2.QuadPart - nLastTime1.QuadPart;
		TRACE( "RecvCost:%f[ms]\r\n", 1000 * ( fInterval / (double)nFreq.QuadPart ) );

		TRACE( "CPU_Frequency:%d\r\n", CPU_Frequency() );
// 		duration = (double)(finish - start);
//		TRACE( "Recv:\t%f\n", duration );
#endif /* RECV_THREAD_TIME_COST */
//		TRACE( "    <-Recv\n" );
		g_ccsRWData.Unlock();
	}

//	TRACE( "Leave RecvThreadFunc()\n" );
	return 0;
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
	__asm mov DWORD PTR iStartCounter, EAX
	__asm mov DWORD PTR (iStartCounter+4), EDX

		while(CurrTicks.QuadPart<TicksCount.QuadPart)
			QueryPerformanceCounter(&CurrTicks);

	__asm rdtsc
	__asm mov DWORD PTR iStopCounter, EAX
	__asm mov DWORD PTR (iStopCounter + 4), EDX

		SetThreadPriority(GetCurrentThread() , dwOldThreadP);
	SetPriorityClass(GetCurrentProcess() , dwOldProcessP);

	return (int)((iStopCounter-iStartCounter)/62500);
}