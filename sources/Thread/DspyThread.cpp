/****************************************************************************/
/*	Virtual Oscilloscopes Upper Computer Application						*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	File Name	:	DspyThread.cpp											*/
/*	Coder		:	sun.yt													*/
/*	Purpose		:															*/
/*																			*/
/****************************************************************************/

#pragma once

/*-----------------------------------------------------------> For Debug <--*/

/*------------------------------------------------------------> Inclueds <--*/
#include "DspyThread.h"
#include "..\..\sources\Dialog\Resource.h"
#include "..\..\sources\OpenGL\OpenGLFunc.h"
#include "..\..\sources\common\Global.h"

/*----------------------------------------------------------> Prototypes <--*/

/*--------------------------------------------------> External Functions <--*/

/*--------------------------------------------------> External Variables <--*/

/*-------------------------------------------------------> Local Defines <--*/

/*-----------------------------------------------------> Local Variables <--*/

/*-----------------------------------------------------------> Constants <--*/

//-----------------------------------------------------------------------------
// Name: DspyThreadFunc
// Desc: 
//-----------------------------------------------------------------------------
UINT DspyThreadFunc( LPVOID lpParam )
{
	//	TRACE( "Enter DspyThreadFunc()\n" );
	SDspyThreadInfo  *pInfo = ( SDspyThreadInfo * )lpParam;
	bool	*pbClearFlag = &( pInfo->bClearFlag );
	bool	bClearFlag	 = false;
	int		xCnt;

	int			*pm_nXCnt				= &( g_pMainThis->m_nXCnt );
	int			*pm_nXCntOffset			= &( g_pMainThis->m_nXCntOffset );
	int			*pm_nSampPerFrame		= &( g_pMainThis->m_nSampPerFrame );
	int			*pm_nNumOfDiv			= &( g_pMainThis->m_nDivNum );
	float		*pm_fXPosition			= &( g_pMainThis->m_fXPosition );
	float		*pm_fYPositionA			= &( g_pMainThis->m_fYPositionA );
	BYTE		*pm_byScaleA			= &( g_pMainThis->m_byScaleA );
	long		*pm_nQueueRearOffset	= &( g_pMainThis->m_nQueueRearOffset );
	CScrollBar	*pm_ctrlSbDrawFrame		= &( g_pMainThis->m_ctrlSbDrawFrame );

	wglMakeCurrent( g_COGL.m_hDC, g_COGL.m_hRC1 );

	while ( 1 )
	{
		WaitForSingleObject( g_eventDspy.m_hObject, INFINITE );
		if ( false == g_COGL.m_bDspyThreadRunning )
		{
			break;
		}
		g_ccsRWData.Lock();
		//		TRACE( "->Dspy\n" );
		//		start = clock();

		*pm_nXCnt %= *pm_nSampPerFrame;	// make sure m_nXCnt < m_nSampPerFrame   
		// when m_nSampPerFrameis reduced in 
		// spneditTimebaseScale()

		xCnt = *pm_nXCnt + *pm_nXCntOffset + 1;			// add 1 so that fill the display area

		if ( *pm_nQueueRearOffset < 0 )	// when the nPosition is smaller than  
		{								// m_nDivNum, m_nQueueRearOffset != 0
			xCnt = *pm_nSampPerFrame;
		}

		bClearFlag = *pbClearFlag;
		if ( xCnt > MAX_SAMP_RECORD )
		{
			xCnt = MAX_SAMP_RECORD;
			bClearFlag = true;
		}

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		if ( bClearFlag || ( 0 == *pm_nXCnt ) )
		{
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			//			TRACE( "%d\n", ( *pbClearFlag ? 1 : 0 ) );
		}

		g_COGL.DrawGrid( GLLEFT, GLRIGHT, GLBOTTOM, GLTOP );

		glTranslatef( *pm_fXPosition * GLWITDH / 12, 
			*pm_fYPositionA * GLHEIGHT / 6 + 150.0f, 
			0.1f );
		glScalef( ( GLWITDH / *pm_nSampPerFrame ), 
			1 / c_fVSCoef[ *pm_byScaleA ], 1.0f );
		glColor3ub( 255, 0, 0 );

		long x = g_psQueue->rear + *pm_nQueueRearOffset;
		// 		xCnt = (int)fSampPerFrame < g_psQueue->num ? (int)fSampPerFrame : g_psQueue->num;
		//		TRACE( "\txCnt:%d", xCnt );
		glBegin( GL_LINE_STRIP );
		{
			for( xCnt--; xCnt >= 0 ; xCnt-- )
			{
				glVertex2f( (GLfloat)xCnt, (GLfloat)g_psQueue->nData[x] );
				x--;
				if ( -1 == x )
				{
					x = MAX_SAMP_RECORD - 1;
				}
			}
		}
		glEnd();

		glFinish();

		// 		finish = clock();
		// 		duration = (double)(finish - start);
		// 		TRACE( "glClear:\t%f\n", duration );
		//		TRACE( "<-Dspy\n" );
		g_ccsRWData.Unlock();
	}

	wglMakeCurrent( NULL, NULL );

	//	TRACE( "Leave DspyThreadFunc()\n" );
	return 0;
}