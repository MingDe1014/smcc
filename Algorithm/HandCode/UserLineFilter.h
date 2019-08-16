/*
 * UserLineFilter.h
 *
 *  Created on: 2019. 6. 8.
 *      Author: user
 */

#ifndef USERLINEFILTER_H_
#define USERLINEFILTER_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "BasicLineScan.h"
#include "BasicPort.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct{
	struct{
		/************************Norm****************************/
		uint32 LadcNorm[128];
		uint32 Max_0, Min_0;	//fixed max&min
		uint32 den0;

		/************************Filter****************************/


	}LeftCam;	//AN_10

	struct{
		/************************Norm****************************/
		uint32 RadcNorm[128];
		uint32 Max_1, Min_1;	//fixed max&min
		uint32 den1;


	}RightCam;	//AN_11

}LineNorm;

typedef struct{

	uint16 avgLeftLine, avgRightLine;
	uint8 LeftLinecnt, RightLinecnt;
	sint32 FiltLeft[128];
	sint32 FiltRight[128];

	uint8 speedrace_flag;
}LineDetect;

typedef struct{
   uint8 nowPosition;
   uint8 avoidanceStage;
   uint8 cntObstacle;
   sint16 compare;
}LineSchoolz;
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN LineNorm g_LineNorm;
IFX_EXTERN LineDetect g_LineDetc;
IFX_EXTERN LineSchoolz g_LineSchoolz;


/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void uNorm(void);
IFX_EXTERN void uLineFilter(void);
IFX_EXTERN void avgLane(void);
IFX_EXTERN void Schoolz_check1(void);
IFX_EXTERN void Schoolz_check2(void);
IFX_EXTERN void Schoolz_checkPosition(void);


#endif /* 0_SRC_APPSW_TRICORE_ALGORITHM_HANDCODE_USERLINEFILTER_H_ */
