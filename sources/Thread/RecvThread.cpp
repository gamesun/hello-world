
#include "RecvThread.h"
#include "..\..\sources\common\Global.h"





//-----------------------------------------------------------------------------
// Name: RecvThreadFunc
// Desc: 
//-----------------------------------------------------------------------------
UINT RecvThreadFunc( LPVOID lpParam )
{
//	TRACE( "Enter RecvThreadFunc()\r\n" );
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
//		TRACE( "    Ask->Recv\r\n" );
		g_ccsRWData.Lock();
//		TRACE( "    ->Recv\r\n" );

#if OSCILL_DEBUG_RECV_TIME_COST
/*
		LARGE_INTEGER nFreq;
		LARGE_INTEGER nLastTime1;
		LARGE_INTEGER nLastTime2;

		QueryPerformanceFrequency( &nFreq );
//		TRACE("Frequency:%I64d\r\n", nFreq );

		QueryPerformanceCounter( &nLastTime1 );
*/
		long   HighStart,LowStart,HighEnd,LowEnd; 
		long   numhigh,numlow;

		__asm 
		{ 
			RDTSC   
			mov		HighStart,	edx 
			mov		LowStart,	eax	
		}
#endif /* OSCILL_DEBUG_RECV_TIME_COST */
/*	
		variant_inp = g_pMainThis->m_mscom.GetInput();
		
		safearray_inp = variant_inp;
		len = safearray_inp.GetOneDimSize();
		for ( i = 0; i < len; i++ )
		{
			safearray_inp.GetElement( &i, &szRxData[i] );
//			szRxData[i] &= 0xff;
			
			nTmpL = szRxData[0] & 0xff;
//			TRACE( "L %02x\r\n", nTmpL );
			INSQueue( g_psQueue, nTmpL );
			(*pm_nXCnt)++;
//			TRACE( "0x%08x ", szRxData[i] );
		}
		
//		szRxData[i] = '\0';
//		TRACE( "%s \r\n", szRxData );
//		TRACE( "\r\n" );
		


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

#if OSCILL_DEBUG_RECV_TIME_COST

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
//		__int64 cost = (numhigh<<32) + numlow;
		
		TRACE( "RecvCost:%f[us]\r\n", ( numlow / g_fDbgCpuFrequency ) );
//		TRACE( "CPU_Frequency:%d\r\n", CPU_Frequency() );
/*
		QueryPerformanceCounter( &nLastTime2 );

//		TRACE( "LastTime1:%I64d\r\n", nLastTime1 );
//		TRACE( "LastTime2:%I64d\r\n", nLastTime2 );

		double fInterval = nLastTime2.QuadPart - nLastTime1.QuadPart;
		TRACE( "RecvCost:%f[ms]\r\n", 1000 * ( fInterval / (double)nFreq.QuadPart ) );
*/
#endif /* OSCILL_DEBUG_RECV_TIME_COST */
//		TRACE( "    <-Recv\r\n" );
		g_ccsRWData.Unlock();
	}

	TRACE( "Leave RecvThreadFunc()\r\n" );
	return 0;
}
