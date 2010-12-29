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
#include "StdAfx.h"

/*----------------------------------------------------------> Prototypes <--*/
int EnumSerialPort( void );

/*--------------------------------------------------> External Functions <--*/

/*--------------------------------------------------> External Variables <--*/

/*-------------------------------------------------------> Local Defines <--*/

/*-----------------------------------------------------> Local Variables <--*/

/*-----------------------------------------------------------> Constants <--*/
typedef struct tagSERIALPORTINFO
{
	CString	szSerialPortList[];
	int		nNum;
}SERIALPORTINFO;


#endif /* __ENUMSERIALPORT_H__ */