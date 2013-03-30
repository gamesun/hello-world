
#pragma once

#ifndef __ENUMSERIALPORT_H__
#define __ENUMSERIALPORT_H__


#include "..\..\sources\Dialog\StdAfx.h"


typedef struct tagSERIALPORTINFO
{
	CString	pszList[256];
	int		nNum;
}SERIALPORTINFO;


SERIALPORTINFO * EnumSerialPort( void );



#endif /* __ENUMSERIALPORT_H__ */