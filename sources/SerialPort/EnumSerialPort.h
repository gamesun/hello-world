/****************************************************************************/
/*	Virtual Oscilloscopes Upper Computer Application						*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	File Name	:	EnumSerialPort.h										*/
/*	Coder		:	sun.yt													*/
/*	Purpose		:															*/
/*																			*/
/****************************************************************************/

#pragma once

#ifndef __ENUMSERIALPORT_H__
#define __ENUMSERIALPORT_H__

/*-----------------------------------------------------------> For Debug <--*/

/*------------------------------------------------------------> Inclueds <--*/
#include "..\..\sources\Dialog\StdAfx.h"

/*----------------------------------------------------------> Structures <--*/
typedef struct tagSERIALPORTINFO
{
	CString	pszList[256];
	int		nNum;
}SERIALPORTINFO;

/*----------------------------------------------------------> Prototypes <--*/
SERIALPORTINFO * EnumSerialPort( void );

/*--------------------------------------------------> External Functions <--*/

/*--------------------------------------------------> External Variables <--*/

/*-------------------------------------------------------> Local Defines <--*/

/*-----------------------------------------------------> Local Variables <--*/

/*-----------------------------------------------------------> Constants <--*/



#endif /* __ENUMSERIALPORT_H__ */