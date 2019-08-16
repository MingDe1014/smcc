/**
 * \file BasicVadcBgScan.h
 * \brief  BasicVadcBgScan
 *
 */

#ifndef BASICVADCBGSCAN_H
#define BASICVADCBGSCAN_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <Vadc/Std/IfxVadc.h>
#include <Vadc/Adc/IfxVadc_Adc.h>
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define IR_getUsrAdcChn0()	IR_AdcResult[0]
#define IR_getUsrAdcChn1()	IR_AdcResult[1]
#define IR_getUsrAdcChn2()	IR_AdcResult[2]
#define IR_getUsrAdcChn3()	IR_AdcResult[3]
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*----------------------------User Data Structures----------------------------*/
/******************************************************************************/
typedef struct{
	float32 distLong;
	float32 distShort;
	uint8 stop_state;
}PSD;

typedef struct{
	float32 threshold1;
	float32 threshold2;
	float32 threshold3;
	float32 threshold4;
	float32 VELOCITY1;
	float32 VELOCITY2;
	float32 VELOCITY3;
	float32 STOP;
	uint8 riskLevel;
}AEBterm;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN float32 IR_AdcResult[];

	/* Usage: IR_AdcResult[USR_ADC_CHN_15_IDX] */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void BasicVadcBgScan_init(void);
IFX_EXTERN void BasicVadcBgScan_run(void);


/******************************************************************************/
/*-------------------------User Function Prototypes---------------------------*/
/******************************************************************************/

IFX_EXTERN void VadcPSDcm(void);

/******************************************************************************/
/*---------------------------User Global variables----------------------------*/
/******************************************************************************/
IFX_EXTERN PSD g_PSD;
IFX_EXTERN AEBterm g_AEBterm;

#endif