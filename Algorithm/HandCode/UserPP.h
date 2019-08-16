/*
 * UserPP.h
 *
 *  Created on: 2019. 6. 8.
 *      Author: user
 */

#ifndef USERPP_H_
#define USERPP_H_

#include "BasicPort.h"
#include "UserLineFilter.h"
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define SCHOOLZ_RIGHT 0
#define SCHOOLZ_LEFT 1

#define NO_OBSTACLE 0
#define OBSTACLE_AVOID 1
#define OBSTACLE_FOLLOW_LINE 2
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*---------------------------User Data Structures-----------------------------*/
/******************************************************************************/

typedef struct{
	uint8 LinePosition;	//0-both cam, 1-left, 2-right
	uint8 hillDetc;
	uint8 hillEnd;
}PP;




/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
//IFX_EXTERN uint8 schoolz_flag; //0(first speed racing), 1(in school zone now), 2(second speed racing)

IFX_EXTERN PP g_PP;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void PurePursuit(void);

IFX_EXTERN void FollowLeftLine(void);	//main camera
IFX_EXTERN void FollowRightLine(void);
IFX_EXTERN void ControlAngle(void);

IFX_EXTERN void FollowOneLine();
IFX_EXTERN void AvoidObstacle(void);

IFX_EXTERN void HillRun(void);
IFX_EXTERN void AEB(void);

#endif /* 0_SRC_APPSW_TRICORE_ALGORITHM_HANDCODE_USERPP_H_ */
