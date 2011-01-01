// Queue.h
//
#pragma once

#include "..\..\sources\Dialog\StdAfx.h"

#define MAX_SAMP_RECORD		10000

typedef  unsigned char QUDA;

struct SQueue
{
	QUDA	nData[MAX_SAMP_RECORD];
	long	rear;
	int		front;
	int		num;
};

SQueue	*InitSQueue( void );
int		INSQueue( SQueue *sq, QUDA nSrcData );

//extern int OutSQueue( SQueue *sq, QUDA *x );
