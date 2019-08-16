/**
 * \file VadcBackgroundScanDemo.c
 * \brief Demo VadcBackgroundScanDemo
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <Cpu/Std/IfxCpu.h>
#include "BasicVadcBgScan.h"
#include "Configuration.h"
#include "UserPP.h"
#include "InfineonRacer.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define  ADC_CHN_MAX  4

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    IfxVadc_Adc vadc;   /* VADC handle*/
    IfxVadc_Adc_Group adcGroup;
    IfxVadc_Adc_Channel adcChannel[ADC_CHN_MAX];
} Basic_VadcBackgroundScan;


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
Basic_VadcBackgroundScan g_VadcBackgroundScan; /**< \brief Demo information */
PSD g_PSD;

AEBterm g_AEBterm = {
		.threshold1 = 1.1,
		.threshold2 = 1.2,
		.threshold3 = 1.35,
		.threshold4 = 1.5,
		.VELOCITY1 = SPEEDRACE*0.8,
		.VELOCITY2 = SPEEDRACE*0.5,
		.VELOCITY3 = SPEEDRACE*0.3,
		.STOP = 0.0
};

static uint8 stop_state0=0, stop_state1=0;

#if BOARD == APPLICATION_KIT_TC237
static uint32 adcChannelNum[ADC_CHN_MAX] = {
		3, 4, 8, 9  /* AN15, AN16, AN20, AN21 */
};
#elif BOARD == SHIELD_BUDDY
static uint32 adcChannelNum[ADC_CHN_MAX] = {
		4, 5, 6, 7
};
#endif


float32 IR_AdcResult[ADC_CHN_MAX];


/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void BasicVadcBgScan_init(void)
{
    /* VADC Configuration */

    /* create configuration */
    IfxVadc_Adc_Config adcConfig;
    IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

    /* initialize module */
    IfxVadc_Adc_initModule(&g_VadcBackgroundScan.vadc, &adcConfig);

    /* create group config */
    IfxVadc_Adc_GroupConfig adcGroupConfig;
    IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &g_VadcBackgroundScan.vadc);

#if BOARD == APPLICATION_KIT_TC237
    /* with group 1 */
    adcGroupConfig.groupId = IfxVadc_GroupId_1;
    adcGroupConfig.master  = adcGroupConfig.groupId;
#elif BOARD == SHIELD_BUDDY
    /* with group 5 */
    adcGroupConfig.groupId = IfxVadc_GroupId_5;
    adcGroupConfig.master  = adcGroupConfig.groupId;
#endif


    /* enable background scan source */
    adcGroupConfig.arbiter.requestSlotBackgroundScanEnabled = TRUE;

    /* enable background auto scan */
    adcGroupConfig.backgroundScanRequest.autoBackgroundScanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    adcGroupConfig.backgroundScanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    /* initialize the group */
    /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
    IfxVadc_Adc_initGroup(&g_VadcBackgroundScan.adcGroup, &adcGroupConfig);

    /* create channel config */
    uint32                    chnIx;

    IfxVadc_Adc_ChannelConfig adcChannelConfig;

    for (chnIx = 0; chnIx < ADC_CHN_MAX; ++chnIx)
    {
        IfxVadc_Adc_initChannelConfig(&adcChannelConfig, &g_VadcBackgroundScan.adcGroup);

        adcChannelConfig.channelId         = (IfxVadc_ChannelId)(adcChannelNum[chnIx]);
        adcChannelConfig.resultRegister    = (IfxVadc_ChannelResult)(adcChannelNum[chnIx]); // use register #5 and 6 for results
        adcChannelConfig.backgroundChannel = TRUE;

        /* initialize the channel */
        IfxVadc_Adc_initChannel(&g_VadcBackgroundScan.adcChannel[chnIx], &adcChannelConfig);

        /* add to background scan */
        unsigned channels = (1 << adcChannelConfig.channelId);
        unsigned mask     = channels;
        IfxVadc_Adc_setBackgroundScan(&g_VadcBackgroundScan.vadc, &g_VadcBackgroundScan.adcGroup, channels, mask);
    }

    /* start scan */
    IfxVadc_Adc_startBackgroundScan(&g_VadcBackgroundScan.vadc);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void BasicVadcBgScan_run(void)
{
    uint32                    chnIx;
    /* wait for valid result */
    volatile Ifx_VADC_RES conversionResult;

        /* check results */
        for (chnIx = 0; chnIx < ADC_CHN_MAX; ++chnIx)
        {
            do
            {
                conversionResult = IfxVadc_Adc_getResult(&g_VadcBackgroundScan.adcChannel[chnIx]);
            } while (!conversionResult.B.VF);

			IR_AdcResult[chnIx] = (float32) conversionResult.B.RESULT / 4095;

        }

        VadcPSDcm();
}



/** \brief Demo run API
 *
 * This function is called ,to convert dV to cm, dV0 : Long, dV1 : Short
 */
void VadcPSDcm(void)
{
	//0: long, 1:short
	float32 dV0=0.0, dV1=0.0;
	float32 temp1=0.0;

	dV0 = 5.0*IR_AdcResult[0];
	dV1 = 5.0*IR_AdcResult[1];

	//temp0 = 19.563*dV0*dV0*dV0*dV0*-164.94*dV0*dV0*dV0+510.19*dV0*dV0-702.4*dV0+399.94;//having a problem
	temp1 = 3.9366*dV1*dV1*dV1*dV1-34.5071*dV1*dV1*dV1+110.7535*dV1*dV1-162.7493*dV1+109.3686;

	g_PSD.distLong = dV0;
	g_PSD.distShort = temp1;

	if(g_PSD.distLong < 1.3 || g_PSD.distLong == 0)
	{  //55cm 밖일때
		stop_state0 = 0;

		if(g_PP.hillDetc == 1)
		{
			g_PP.hillEnd = 1;
		}
	}
	/*
	else if(g_PSD.distLong == 0){
		stop_state0 = 0;
	}*/
	else{
		stop_state0 = 1;
	}

	if(g_PSD.distShort > 28 || g_PSD.distShort <= 0){  //25cm 밖일때
		stop_state1 = 0;
	}
	/*
	else if(g_PSD.distShort == 0 || g_PSD.distShort < 0){
		stop_state1 = 0;
	}*/
	else{
		stop_state1 = 1;
	}

	g_PSD.stop_state = stop_state0 + stop_state1;

	/************************* 본선 - school zone *************************/
	if(g_LineDetc.speedrace_flag == 1)
	{
		switch(g_LineSchoolz.avoidanceStage){
			case NO_OBSTACLE:
				if(g_PSD.stop_state != 0)
					g_LineSchoolz.avoidanceStage = OBSTACLE_AVOID;


				break;

			case OBSTACLE_AVOID:
				if(g_PSD.stop_state == 0){
					g_LineSchoolz.avoidanceStage = OBSTACLE_FOLLOW_LINE;
				}
				break;


			case OBSTACLE_FOLLOW_LINE:
//				if(g_PSD.stop_state == 0 &&){
//					g_LineSchoolz.avoidanceStage = NO_OBSTACLE;
//				}
				break;

			default :
				break;
			}

	}

	/************************* 본선 - hill, AEB *************************/
	else if(g_LineDetc.speedrace_flag == 2)
	{
//		if(g_PP.hillDetc == 0)	//언덕 감지 전
//		{
			if(g_PSD.distLong > 1.3)
			{  //긴 애 55cm 이내일 때
				if(g_PSD.distShort > 40 || g_PSD.distShort <= 0)
				{  //짧은 애 40cm 밖일때
					g_PP.hillDetc = 1;
				}
			}
//		}

//		else	//언덕 감지 후
//		{	/******************** 멀리 감지하는 psd *******************/
			if(g_PSD.distLong < g_AEBterm.threshold1){  //70m 밖일때
				g_AEBterm.riskLevel = 0;
			}
			else if(g_PSD.distLong > g_AEBterm.threshold1 && g_PSD.distLong < g_AEBterm.threshold2){  //
				g_AEBterm.riskLevel = 1;
			}
			else if(g_PSD.distLong > g_AEBterm.threshold2 && g_PSD.distLong < g_AEBterm.threshold3){  //
				g_AEBterm.riskLevel = 2;
			}
			else if(g_PSD.distLong > g_AEBterm.threshold3 && g_PSD.distLong < g_AEBterm.threshold4){  //
				g_AEBterm.riskLevel = 3;
			}
			else if(g_PSD.distLong > g_AEBterm.threshold4){  //
				g_AEBterm.riskLevel = 4;
			}

			/******************** 짧게 감지하는 psd *******************/
			if(g_PSD.distShort < 25 && g_PSD.distShort >= 0){  //25m 밖일때
				g_AEBterm.riskLevel = 4;
			}

//		}

	}
}
