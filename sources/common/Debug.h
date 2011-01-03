

#pragma once

#ifndef __DEBUG_H__
#define __DEBUG_H__

//time debug
#define OSCILL_DEBUG_RECV_TIME_COST		0
#define OSCILL_DEBUG_XXXX				0

#define OSCILL_DEBUG				( OSCILL_DEBUG_RECV_TIME_COST \
									| OSCILL_DEBUG_XXXX )




#if OSCILL_DEBUG_RECV_TIME_COST
extern double	g_fDbgCpuFrequency;

extern int CPU_Frequency( void );
#endif

#if OSCILL_DEBUG_XXXX

#endif

extern void ShowLastError( LPTSTR lpszFunction );

#endif /* __DEBUG_H__ */