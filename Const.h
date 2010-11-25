// Const.h : header file
//

#pragma once

#include "StdAfx.h"

#ifndef __CONST_H__
#define __CONST_H__


#define TIMEBASESCALE_NUM   16
static const char *pc_szTbScale[TIMEBASESCALE_NUM] = 
{   
    "10us/Div",
    "20us/Div",
    "50us/Div",
    "100us/Div",
    "200us/Div",
    "500us/Div",
    "1ms/Div",
    "2ms/Div",
    "5ms/Div",
    "10ms/Div",
    "20ms/Div",
    "50ms/Div",
    "100ms/Div",
    "200ms/Div",
    "500ms/Div",
    "1s/Div",

};

static const float c_fTbScaleCoef[TIMEBASESCALE_NUM] = 
{									// buf div length
/* 0 */	0.00001f,	// 	"10us/Div",		2000
/* 1 */	0.00002f,	// 	"20us/Div",		
/* 2 */	0.00005f,	// 	"50us/Div",		
/* 3 */	0.0001f,	// 	"100us/Div",	
/* 4 */	0.0002f,	// 	"200us/Div",	
/* 5 */	0.0005f,	// 	"500us/Div",	
/* 6 */	0.001f,		// 	"1ms/Div",		
/* 7 */	0.002f,		// 	"2ms/Div",		
/* 8 */	0.005f,		// 	"5ms/Div",		4
/* 9 */	0.01f,		// 	"10ms/Div",		2
/*10 */	0.02f,		// 	"20ms/Div",		1
/*11 */	0.05f,		// 	"50ms/Div",		
/*12 */	0.1f,		// 	"100ms/Div",	
/*13 */	0.2f,		// 	"200ms/Div",	
/*14 */	0.5f,		// 	"500ms/Div",	
/*15 */	1.0f,		// 	"1s/Div",		
};

#define VOLTAGESCALE_NUM    16
static const char *pc_szVoltScale[VOLTAGESCALE_NUM] = 
{   
    "1mV/Div",
    "2mV/Div",
    "5mV/Div",
    "10mV/Div",
    "20mV/Div",
    "50mV/Div",
    "100mV/Div",
    "200mV/Div",
    "500mV/Div",
    "1V/Div",
    "2V/Div",
    "5V/Div",
    "10V/Div",
    "20V/Div",
    "50V/Div",
    "100V/Div",
};

static const float c_fVSCoef[VOLTAGESCALE_NUM] = 
{
	0.001f,			// "1mV/Div",
	0.002f,			// "2mV/Div",
	0.005f,			// "5mV/Div",
	0.01f,			// "10mV/Div",
	0.02f,			// "20mV/Div",
	0.05f,			// "50mV/Div",
	0.1f,			// "100mV/Div",
	0.2f,			// "200mV/Div",
	0.5f,			// "500mV/Div",
	1.0f,			// "1V/Div",
	2.0f,			// "2V/Div",
	5.0f,			// "5V/Div",
	10.0f,			// "10V/Div",
	20.0f,			// "20V/Div",
	50.0f,			// "50V/Div",
	100.0f,			// "100V/Div",	
};

static const SHORT g_nTest[200] = 
{
	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	
	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	
	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	
	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	
	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	
	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	
	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	
	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	
	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	
	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	
	101,	102,	103,	104,	105,	106,	107,	108,	109,	110,	
	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	
	121,	122,	123,	124,	125,	126,	127,	128,	129,	130,	
	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,	
	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	
	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	
	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	
	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	
	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	
	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	
};


#endif /* __CONST_H__ */