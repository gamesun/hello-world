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

/*-----------------------------------------------------------> For Debug <--*/

/*------------------------------------------------------------> Inclueds <--*/
#include "RecvThread.h"
#include "..\..\sources\common\Global.h"

/*----------------------------------------------------------> Prototypes <--*/

/*--------------------------------------------------> External Functions <--*/

/*--------------------------------------------------> External Variables <--*/
//extern COscillDlg *g_pMainThis;

/*-------------------------------------------------------> Local Defines <--*/

/*-----------------------------------------------------> Local Variables <--*/

/*-----------------------------------------------------------> Constants <--*/

//-----------------------------------------------------------------------------
// Name: RecvThreadFunc
// Desc: 
//-----------------------------------------------------------------------------
UINT RecvThreadFunc( LPVOID lpParam )
{
//	TRACE( "Enter RecvThreadFunc()\n" );
	int				noldPos;
//	bool			bRecvDigitHigh;
//	SHORT			nTmpH;
	char			nTmpL;
	long			i;
	int				len;
//	char			szRxData[1024];
//	VARIANT			variant_inp;
	COleSafeArray	safearray_inp;
	
	noldPos			= 0;
//	bRecvDigitHigh	= true;
//	nTmpH			= 0;
	nTmpL			= 0;
	i				= 0;
	len				= 0;

	int			*pm_nXCnt				= &g_pMainThis->m_nXCnt;
	int			*pm_nSampPerFrame		= &g_pMainThis->m_nSampPerFrame;
	int			*pm_nNumOfDiv			= &g_pMainThis->m_nDivNum;
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
		g_ccsRWData.Lock();
//		TRACE( "    ->Recv\n" );
//		start = clock();

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
		SendMessage( GetDlgItem( g_pMainThis->m_hWnd, IDC_SBDRAWFRAME ), 
					SBM_SETRANGE, FALSE, *pm_nNumOfDiv );
		g_pMainThis->m_ctrlSbDrawFrame.SetScrollRange( 0, *pm_nNumOfDiv );
		
		noldPos = pm_ctrlSbDrawFrame->GetScrollPos();
		pm_ctrlSbDrawFrame->SetScrollPos( *pm_nNumOfDiv, FALSE );
		
		if ( noldPos != *pm_nNumOfDiv )
		{
			*pm_nQueueRearOffset = 0;
			g_pMainThis->UpdateFrame( CLEAR );
		}
		else
		{
			g_pMainThis->UpdateFrame( NO_CLEAR );
		}

// 		finish = clock();
// 		duration = (double)(finish - start);
//  		TRACE( "Recv:\t%f\n", duration );
//		TRACE( "    <-Recv\n" );
		g_ccsRWData.Unlock();
	}

//	TRACE( "Leave RecvThreadFunc()\n" );
	return 0;
}
