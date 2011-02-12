

#include "DspyThread.h"
#include "..\..\sources\Dialog\Resource.h"
#include "..\..\sources\OpenGL\OpenGLFunc.h"
#include "..\..\sources\common\Global.h"
#include "..\..\sources\common\Const.h"


//-----------------------------------------------------------------------------
// Name: DspyThreadFunc
// Desc: 
//-----------------------------------------------------------------------------
UINT DspyThreadFunc( LPVOID lpParam )
{
	//	TRACE( "Enter DspyThreadFunc()\r\n" );
	SDTPARA		*pInfo;
	BYTE		*pbyDrawMask;
	BYTE		byDrawMask;
	bool		bClearFlag;
	int			xCnt;

	int			*pm_nXCnt;
	int			*pm_nXCntOffset;
	int			*pm_nSampPerFrame;
	int			*pm_nNumOfDiv;
	BYTE		*pm_byTbScale;
	float		*pm_fXPosition;
	float		*pm_fYPositionA;
	BYTE		*pm_byScaleA;
	long		*pm_nQueueRearOffset;
	CScrollBar	*pm_ctrlSbDrawFrame;

	pInfo				= (SDTPARA *)lpParam;
	pbyDrawMask			= &( pInfo->byDrawMask );
	byDrawMask			= *pbyDrawMask;
	bClearFlag			= false;

	pm_nXCnt			= &( g_pMainThis->m_nXCnt );
	pm_nXCntOffset		= &( g_pMainThis->m_nXCntOffset );
	pm_nSampPerFrame	= &( g_pMainThis->m_nSampPerFrame );
	pm_nNumOfDiv		= &( g_pMainThis->m_nNumOfDiv );
	pm_byTbScale		= &( g_pMainThis->m_byTbScale );
	pm_fXPosition		= &( g_pMainThis->m_fXPosition );
	pm_fYPositionA		= &( g_pMainThis->m_fYPositionA );
	pm_byScaleA			= &( g_pMainThis->m_byScaleA );
	pm_nQueueRearOffset	= &( g_pMainThis->m_nQueueRearOffset );
	pm_ctrlSbDrawFrame	= &( g_pMainThis->m_ctrlSbDrawFrame );

	wglMakeCurrent( g_COGL.m_hDC, g_COGL.m_hRC1 );

	while ( 1 )
	{
		WaitForSingleObject( g_eventDspy.m_hObject, INFINITE );
		if ( false == g_COGL.m_bDspyThreadRunning )
		{
			break;
		}

		byDrawMask = *pbyDrawMask;		//get the para

//		TRACE( "Ask->Dspy\r\n" );
		g_csRWData.Lock();
//		TRACE( "->Dspy\r\n" );
		//		start = clock();
		
		
//////////////////////////////////////////////////////////////////////////
		//	TRACE( ">>Enter Calculate\r\n" );
		if ( 0 != (BYTE)( byDrawMask & CALCULATE ) )
		{
//			TRACE( "        Ask->Calc\r\n" );
			// 	g_csRWData.Lock();
			//			TRACE( "        ->Calc\r\n" );

			int nSampPerDiv = (int)( g_sMeasPara.nSampFreq * c_fTbScaleCoef[*pm_byTbScale] );
			*pm_nSampPerFrame = 12 * nSampPerDiv;

			//	m_nXCnt %= m_nSampPerFrame;
			*pm_nXCnt = g_psQueue->num % *pm_nSampPerFrame;
			*pm_nNumOfDiv = ( g_psQueue->num - *pm_nXCnt ) / nSampPerDiv;
			pm_ctrlSbDrawFrame->SetScrollRange( 0, *pm_nNumOfDiv );

			int nPosition = pm_ctrlSbDrawFrame->GetScrollPos();

			int ntmp = *pm_nNumOfDiv - nPosition - ( 12 - *pm_nXCnt / nSampPerDiv );
			if ( 0 < ntmp )
			{
				*pm_nQueueRearOffset = - *pm_nXCnt % nSampPerDiv - nSampPerDiv * ntmp;
			}
			else
			{
				*pm_nQueueRearOffset = 0;
			}

			if ( nPosition == *pm_nNumOfDiv )
			{
				*pm_nQueueRearOffset = 0;
			}

			*pm_nXCntOffset = 0;
			if ( 0 == *pm_nQueueRearOffset )
			{
				*pm_nXCntOffset = ( *pm_nNumOfDiv - nPosition ) * nSampPerDiv;
			}

/*
			TRACE( "SampPerFrame:%5d DivNum:%3d Pos:%3d NUM:%5d QueueRearOffset:%6d XCnt:%5d XCntOffset:%3d\r\n", 
					  m_nSampPerFrame, 
					  m_nNumOfDiv,
					  nPosition,
					  g_psQueue->num,
					  m_nQueueRearOffset,
					  m_nXCnt,
					  m_nXCntOffset
					  );
*/
//			TRACE( "        <-Calc\r\n" );
		}
//	g_csRWData.Unlock();
//	TRACE( "  >>Leave Calculate\r\n" );
//////////////////////////////////////////////////////////////////////////

		*pm_nXCnt %= *pm_nSampPerFrame;	// make sure m_nXCnt < m_nSampPerFrame   
		// when m_nSampPerFrameis reduced in 
		// spneditTimebaseScale()

		xCnt = *pm_nXCnt + *pm_nXCntOffset + 1;			// add 1 so that fill the display area

		if ( *pm_nQueueRearOffset < 0 )	// when the nPosition is smaller than  
		{								// m_nNumOfDiv, m_nQueueRearOffset != 0
			xCnt = *pm_nSampPerFrame;
		}

		if ( 0 != (BYTE)( byDrawMask & CLEAR ) )
		{
			bClearFlag = true;
		}

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
		}

		g_COGL.DrawGrid( GLLEFT, GLRIGHT, GLBOTTOM, GLTOP );
		g_COGL.DrawRuler( 135 );
		
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
		// 		TRACE( "glClear:\t%f\r\n", duration );
//		TRACE( "<-Dspy\r\n" );
		g_csRWData.Unlock();
	}

	wglMakeCurrent( NULL, NULL );

	TRACE( "Leave DspyThreadFunc()\r\n" );
	return 0;
}