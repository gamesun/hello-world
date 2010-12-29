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
CString EnumSerialPort( void )
{
	CString	strSerialList[256];  //临时定义256个字符串组，因为系统最多也就256个
	HKEY	hKey;
	LPCTSTR	data_Set="HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
	long	ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey));
	
	if ( ret0 != ERROR_SUCCESS )
	{
		return ( -1 );
	}
	
	int i = 0;
	CHAR Name[25];
	UCHAR szPortName[25];
	LONG Status;
	DWORD dwIndex = 0;
	DWORD dwName;
	DWORD dwSizeofPortName;
	DWORD Type;
	dwName = sizeof( Name );
	dwSizeofPortName = sizeof( szPortName );
	
	do
	{
		Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type,
			szPortName, &dwSizeofPortName);

		if((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA))
		{
			strSerialList[i] = CString(szPortName);
			i++;
		}
	} while((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA));
	
	RegCloseKey( hKey );

	return ( i );
}
