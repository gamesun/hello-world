

#pragma once

#ifndef __DEBUG_H__
#define __DEBUG_H__

//time debug
#define OSCILL_DEBUG_TIME_RECV_COST		0
#define OSCILL_DEBUG_TIME_XXXX			0

#define OSCILL_DEBUG_TIME			( OSCILL_DEBUG_TIME_RECV_COST \
									| OSCILL_DEBUG_TIME_XXXX )




#if OSCILL_DEBUG_TIME
extern double	g_fDbgCpuFrequency;

extern int CPU_Frequency( void );
#endif

#if OSCILL_DEBUG_TIME_XXXX

#endif

extern void ShowLastError( LPTSTR lpszFunction );

#endif /* __DEBUG_H__ */