/****************************************************************************/
/*	Virtual Oscilloscopes Upper Computer Application						*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	File Name	:	EnumSerialPort.cpp										*/
/*	Coder		:	sun.yt													*/
/*	Purpose		:															*/
/*																			*/
/****************************************************************************/

#pragma once

/*-----------------------------------------------------------> For Debug <--*/

/*------------------------------------------------------------> Inclueds <--*/
#include "EnumSerialPort.h"
#include <string.h>

/*----------------------------------------------------------> Prototypes <--*/

/*--------------------------------------------------> External Functions <--*/

/*--------------------------------------------------> External Variables <--*/

/*-------------------------------------------------------> Local Defines <--*/

/*-----------------------------------------------------> Local Variables <--*/

/*-----------------------------------------------------------> Constants <--*/

//-----------------------------------------------------------------------------
// Name: EnumSerialPort
// Desc: 
//-----------------------------------------------------------------------------
 SERIALPORTINFO * EnumSerialPort( void )
{
	CString	strSerialList[256];  //临时定义256个字符串组，因为系统最多也就256个
	HKEY	hKey;
	LPCTSTR	data_Set="HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
	long	ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey));
	
	if ( ret0 != ERROR_SUCCESS )
	{
		return ( NULL );
	}
	
	int i;
	CHAR Name[25];
	UCHAR szPortName[25];
	LONG Status;
	DWORD dwIndex = 0;
	DWORD dwName;
	DWORD dwSizeofPortName;
	DWORD Type;
	dwName = sizeof( Name );
	dwSizeofPortName = sizeof( szPortName );
	SERIALPORTINFO	*psSerialPortInfo;
	psSerialPortInfo = new SERIALPORTINFO;
//	memset( psSerialPortInfo, 0, sizeof( SERIALPORTINFO ) );
	psSerialPortInfo->nNum = 0;
	i = 0;
	do
	{
		Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type,
			szPortName, &dwSizeofPortName);

		if((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA))
		{
			psSerialPortInfo->pszList[i] = CString( szPortName );
			psSerialPortInfo->nNum++;
// 			strSerialList[i] = CString(szPortName);
 			i++;
		}
	} while((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA));
	
	RegCloseKey( hKey );

	return ( psSerialPortInfo );
}
