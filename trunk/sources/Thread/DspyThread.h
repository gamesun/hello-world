

#pragma once

#ifndef __DSPYTHREAD_H__
#define __DSPYTHREAD_H__

#include "..\..\sources\Dialog\StdAfx.h"

#define BASIC		(BYTE)1
#define CLEAR		(BYTE)2
#define CALCULATE	(BYTE)4


typedef struct tagSDTPARA	//DspyThread Para
{
	BYTE	byDrawMask;

}SDTPARA;


UINT    DspyThreadFunc( LPVOID lpParam );	//Display


#endif /* __DSPYTHREAD_H__ */