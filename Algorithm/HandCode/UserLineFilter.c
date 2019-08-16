/*
 * UserLineFilter.c
 *
 *  Created on: 2019. 6. 8.
 *      Author: user
 */



/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <stdio.h>
#include "UserLineFilter.h"
#include "UserPP.h"
#include "BasicLineScan.h"
#include "BasicGtmTom.h"
#include "Configuration.h"


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

#define RFILT_THRES 180000//20000 : max least, 7000 : min least, but in max light, noise is bigger than 7000
#define LFILT_THRES 180000//220000 : max least, 150000 : min least
#define NORM_ON 0
#define NORM_OFF 1

const uint16 rawden = 4500;

LineNorm g_LineNorm;
LineDetect g_LineDetc;
LineSchoolz g_LineSchoolz;


uint8 LeftLinePast[4]={0,};	//3 is the newest
uint8 RightLinePast[4]={0,};

uint8 switch0 = 0;
uint32 tempM_0, tempM_1;	//maximum of data
uint32 tempL_0, tempL_1;	//minimum of data


uint16 g_LeftLine, g_RightLine;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void uNorm(void){
	//1:left, 0:right

	uint32 tempR_0=0, tempR_1=0;	//result of normalization
	uint8 idx;

//	switch0 = Switch_State();
//
//	switch(switch0){
//	case NORM_OFF:	//off
//		break;
//
//	case NORM_ON:	//on

	   tempM_0 = IR_LineScan.adcResult[1][0];
	   tempM_1 = IR_LineScan.adcResult[0][0];
	   tempL_0 = IR_LineScan.adcResult[1][0];
	   tempL_1 = IR_LineScan.adcResult[0][0];

	   for(idx = 1; idx < 127; ++idx)
	   {
	      if(tempM_0 <= IR_LineScan.adcResult[1][idx]){tempM_0 = IR_LineScan.adcResult[1][idx];}
	      if(tempM_1 <= IR_LineScan.adcResult[0][idx]){tempM_1 = IR_LineScan.adcResult[0][idx];}
	      if(tempL_0 >= IR_LineScan.adcResult[1][idx]){tempL_0 = IR_LineScan.adcResult[1][idx];}
	      if(tempL_1 >= IR_LineScan.adcResult[0][idx]){tempL_1 = IR_LineScan.adcResult[0][idx];}

	   }

		//IR_setLed0(0);
		//BasicPort_run();
//		break;
//
//	default:
//		break;
//	}

	g_LineNorm.LeftCam.Max_0 = tempM_0;
	g_LineNorm.RightCam.Max_1 = tempM_1;
	g_LineNorm.LeftCam.Min_0 = tempL_0;
	g_LineNorm.RightCam.Min_1 = tempL_1;

	g_LineNorm.LeftCam.den0 = g_LineNorm.LeftCam.Max_0 - g_LineNorm.LeftCam.Min_0;
	g_LineNorm.RightCam.den1 = g_LineNorm.RightCam.Max_1 - g_LineNorm.RightCam.Min_1;

	if(g_LineNorm.LeftCam.den0 == 0){
		g_LineNorm.LeftCam.den0 = rawden;
	}

	if(g_LineNorm.RightCam.den1 == 0){
		g_LineNorm.RightCam.den1 = rawden;
	}

	for(idx = 0; idx < 127; ++idx)
	{
		tempR_0 = (IR_LineScan.adcResult[1][idx]*4500)/g_LineNorm.LeftCam.den0;
		tempR_1 = (IR_LineScan.adcResult[0][idx]*4500)/g_LineNorm.RightCam.den1;
		g_LineNorm.LeftCam.LadcNorm[idx]=tempR_0;
		g_LineNorm.RightCam.RadcNorm[idx]=tempR_1;
	}

}

void uLineFilter(void){

    uint32 idx;
    //sint8 Lweight[9] = {1, 4, 4, -4, -10, -4, 4, 4, 1};	//left is ok
    //sint8 Rweight[9] = {-1, -4, -5, 3, 14, 3, -5, -4, -1}; //right, more blurr
    //sint8 weight[13] = {-1, -8, -26, -40, -15, 48, 84, 48, -15, -40, -26, -8, -1}; // 최종 filtering 계수
    //sint16 weight[13] = {1, 14, 68, 149, 112, 158, -372, 158, 112, 149, 68, 14, 1}; // 최종 filtering 계수


    /*           temporary detecting line                */
    uint16 sumL=0, sumR=0;
    uint8 Leftcnt=0, Rightcnt=0;


    for(idx = 6; idx < 128-6; ++idx) // sample 개수 : 13개
        {
    		g_LineDetc.FiltRight[idx]= -g_LineNorm.RightCam.RadcNorm[idx-6] - 8*g_LineNorm.RightCam.RadcNorm[idx-5] - 27*g_LineNorm.RightCam.RadcNorm[idx-4]
									- 45*g_LineNorm.RightCam.RadcNorm[idx-3] - 21*g_LineNorm.RightCam.RadcNorm[idx-2] + 53*g_LineNorm.RightCam.RadcNorm[idx-1]
									+ 98*g_LineNorm.RightCam.RadcNorm[idx] + 53*g_LineNorm.RightCam.RadcNorm[idx+1] - 21*g_LineNorm.RightCam.RadcNorm[idx+2]
									- 45*g_LineNorm.RightCam.RadcNorm[idx+3] - 27*g_LineNorm.RightCam.RadcNorm[idx+4] - 8*g_LineNorm.RightCam.RadcNorm[idx+5]
									- g_LineNorm.RightCam.RadcNorm[idx+6];

        	g_LineDetc.FiltLeft[idx] = -g_LineNorm.LeftCam.LadcNorm[idx-6] - 8*g_LineNorm.LeftCam.LadcNorm[idx-5] - 27*g_LineNorm.LeftCam.LadcNorm[idx-4]
    									- 45*g_LineNorm.LeftCam.LadcNorm[idx-3] - 21*g_LineNorm.LeftCam.LadcNorm[idx-2] + 53*g_LineNorm.LeftCam.LadcNorm[idx-1]
    									+ 98*g_LineNorm.LeftCam.LadcNorm[idx] + 53*g_LineNorm.LeftCam.LadcNorm[idx+1] - 21*g_LineNorm.LeftCam.LadcNorm[idx+2]
										- 45*g_LineNorm.LeftCam.LadcNorm[idx+3] - 27*g_LineNorm.LeftCam.LadcNorm[idx+4] - 8*g_LineNorm.LeftCam.LadcNorm[idx+5]
										- g_LineNorm.LeftCam.LadcNorm[idx+6];


        }
/*
    for(idx = 4; idx < 128-4; ++idx) // sample 개수 : 13개
        {
        	g_LineDetc.FiltRight[idx]= -2*g_LineNorm.RightCam.RadcNorm[idx-4] - 8*g_LineNorm.RightCam.RadcNorm[idx-3] - 8*g_LineNorm.RightCam.RadcNorm[idx-2]
										+ 8*g_LineNorm.RightCam.RadcNorm[idx-1] + 20*g_LineNorm.RightCam.RadcNorm[idx] + 8*g_LineNorm.RightCam.RadcNorm[idx+1]
										- 8*g_LineNorm.RightCam.RadcNorm[idx+2] - 8*g_LineNorm.RightCam.RadcNorm[idx+3] - 2*g_LineNorm.RightCam.RadcNorm[idx+4];
        }*/

    //temporary_test_in_pp

    for(idx = 10; idx < 117; idx++)
   	{
   		if(g_LineDetc.FiltLeft[idx] > LFILT_THRES){
   			//tempLeft = idx;
   			sumL += idx;
   			Leftcnt++;
   		}
   		if( g_LineDetc.FiltRight[idx] > RFILT_THRES){
    		//tempRight = idx;
   			sumR += idx;
   			Rightcnt++;
   		}
   	}


    //전역적으로 사용될 변수로 값 저장
    /*********************/
    g_LineDetc.LeftLinecnt = Leftcnt;
    g_LineDetc.RightLinecnt = Rightcnt;
    /*********************/

    if(Leftcnt == 0){
    	Leftcnt = 1;
    	sumL = LeftLinePast[3];
    }

    if(Rightcnt == 0){
    	Rightcnt = 1;
    	sumR = RightLinePast[3];
    }

    sumL /= Leftcnt;
   	sumR /= Rightcnt;

   	g_LeftLine = sumL;
   	g_RightLine = sumR;
   	/***************************************************************************/
}
void Schoolz_check1(void)
{
	uint16 _sum_1 = 0;
	_sum_1 = g_LineDetc.LeftLinecnt + g_LineDetc.RightLinecnt;

    //speed->school
    if(_sum_1 > 16){
    	//school zone mode
    	g_LineDetc.speedrace_flag = 1;
    }

}

void Schoolz_check2(void)
{
	uint16 _sum_2 = 0;
	_sum_2 = g_LineDetc.LeftLinecnt + g_LineDetc.RightLinecnt;

	//school->speed
	if(_sum_2 > 16){
		//second speed racing mode
		g_LineDetc.speedrace_flag = 2;
	}
}

//과거 값 의존
void avgLane(void){
	uint8 idx;
	uint16 templeft=0, tempright=0;

	// 값 갱신
	for(idx = 0; idx<3; ++idx)
	{
	    LeftLinePast[idx] = LeftLinePast[idx+1];
	    RightLinePast[idx] = RightLinePast[idx+1];
	}
	LeftLinePast[3] = g_LeftLine;
	RightLinePast[3] = g_RightLine;

	// 평균값 계산
	for(idx = 0; idx<4; ++idx)
	{
		templeft += LeftLinePast[idx];
		tempright += RightLinePast[idx];
	}
	templeft /= 4;
	tempright /= 4;

	g_LineDetc.avgLeftLine = templeft;
	g_LineDetc.avgRightLine = tempright;

}

// 민재 수정중
void Schoolz_checkPosition()
{
	if(g_LineDetc.LeftLinecnt - g_LineDetc.RightLinecnt > 0){
		g_LineSchoolz.compare++;
	}
	else{
		g_LineSchoolz.compare--;
	}


	if(g_LineSchoolz.compare > 10) {//왼쪽일 경우
		g_LineSchoolz.nowPosition = SCHOOLZ_LEFT;
	}
	else if(g_LineSchoolz.compare < -10) {//오른쪽일 경우
		g_LineSchoolz.nowPosition = SCHOOLZ_RIGHT;
	}

}
