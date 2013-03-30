
#pragma once

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "..\..\sources\Dialog\StdAfx.h"
#include "..\..\sources\Dialog\Oscill.h"


extern	BOOL		g_bActive;
extern	COpenGL		g_COGL;
extern	SQueue		*g_psQueue;
extern	SMeasPara	g_sMeasPara;

extern	COscillDlg	*g_pMainThis;

extern	CEvent		g_eventDspy;
extern	CEvent		g_eventCommRecv;
extern	CCriticalSection	g_csRWData;

#endif /* __GLOBAL_H__ */