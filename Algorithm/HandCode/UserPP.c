/*
 * UserPP.c
 *
 *  Created on: 2019. 6. 8.
 *      Author: user
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <stdio.h>

#include <math.h>
#include "UserPP.h"
#include "Configuration.h"
#include "BasicVadcBgScan.h"
#include "BasicGtmTom.h"	//IR_setSrvAngle();

/******************************************************************************/
/*------------------------------User structure------------------------------*/
/******************************************************************************/
typedef struct{
	float64 p;
	float64 i;
	float64 d;
}angleGain;

typedef struct{
	float64 error;
	float64 pre_error;
	float64 integral;
	float64 derivative;
}angleError;

angleGain angle_Gain = {
		.p = 1.3,
		.i = 0.01,
		.d = 0.01
};	//pp -(0.3) p:0.98, i:0.01, d:0.01

angleError angle_Error = {
		.error = 0.0,
		.pre_error = 0.0,
		.integral = 0.0,
		.derivative = 0.0
};

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

PP g_PP;

//left 50 pixel, right 43 pixel : black line
//pixel in real mm - left : 4,  right : 4.65

const float32 wheelbase = 256;	//mm
const float32 pixel_mm = 4.5;


float32 look_ahead_dist = 656;	//mm, front direction
float32 x_point;	//pixel side direction, control

float32 R_dist;
float32 result_angle;

float64 parameter;	//변수
float64 wheelGain;

float32 pre_str_ang1;	//과거 첫번째
float32 pre_str_ang2;	//과거 두번째
float32 pre_str_ang3;	//과거 세번째
float32 pre_str_ang4;	//과거 네번째

float32 avg_steering_angle;

//함수에 들어갈 실제 값
float32 test_angle;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
//LEFT : -1.0~0.13 && RIGHT : 0.13~1.0

void FollowLeftLine()
{
//	//midline = 65
//	if(g_LineDetc.avgLeftLine > 100){//왼쪽 회전
//		result_angle = -1.0;
//	}
//	else if(g_LineDetc.avgLeftLine < 28){//우측 회전
//		result_angle = 1.0;
//	}
//
//	else if(g_LineDetc.avgLeftLine < 68 && g_LineDetc.avgLeftLine > 53){//직선 주행
//		result_angle = 0.0;
////		x_point = (61-g_LineDetc.avgLeftLine)*pixel_mm;
////		R_dist = (look_ahead_dist*look_ahead_dist + x_point*x_point)/(2*x_point);
////		parameter = wheelbase/R_dist;
////		result_angle = 0.04* parameter*(180/IFX_PI);
//	}
//	else{
//		x_point = (65-g_LineDetc.avgLeftLine)*pixel_mm;
//		R_dist = (look_ahead_dist*look_ahead_dist + x_point*x_point)/(2*x_point);
//		parameter = wheelbase/R_dist;
//		result_angle = parameter*(180/IFX_PI);
//	}

	x_point = (65-g_LineDetc.avgLeftLine)*pixel_mm;
	R_dist = (look_ahead_dist*look_ahead_dist + x_point*x_point)/(2*x_point);
	parameter = wheelbase/R_dist;
	result_angle = parameter*(180/IFX_PI);

	ControlAngle();

	if(avg_steering_angle > 10.5){
		IR_setSrvAngle(1.0);
	}

	else if(avg_steering_angle < -10.5){
		IR_setSrvAngle(-1.0);
	}

	else if(avg_steering_angle < 3.0 && avg_steering_angle > -3.0){
		IR_setSrvAngle(0.13);
	}

	else if(avg_steering_angle >= 3.0){//오른쪽 회전
		wheelGain = 0.0989;
		test_angle = wheelGain*avg_steering_angle;
		IR_setSrvAngle(test_angle);
	}

	else{//왼쪽 회전
		wheelGain = 0.0989;
		test_angle = wheelGain*avg_steering_angle;
		IR_setSrvAngle(test_angle);
	}

}

void FollowRightLine()
{
	//middle line 55
//		if(g_LineDetc.avgLeftLine > 96){//왼쪽 회전
//			result_angle = -1.0;
//		}
//		else if(g_LineDetc.avgLeftLine < 28){//우측 회전
//			result_angle = 1.0;
//		}
//
//		else if(g_LineDetc.avgLeftLine < 72 && g_LineDetc.avgLeftLine > 58){//직선 주행
//			result_angle = 0.0;
//		}
//
//		else{
//			x_point = (60-g_LineDetc.avgLeftLine)*pixel_mm;
//			R_dist = (look_ahead_dist*look_ahead_dist + x_point*x_point)/(2*x_point);
//			parameter = wheelbase/R_dist;
//			result_angle = parameter*(180/IFX_PI);
//		}

		x_point = (55-g_LineDetc.avgRightLine)*pixel_mm;
		R_dist = (look_ahead_dist*look_ahead_dist + x_point*x_point)/(2*x_point);
		parameter = wheelbase/R_dist;
		result_angle = parameter*(180/IFX_PI);

		ControlAngle();

		if(avg_steering_angle > 10.5){
			IR_setSrvAngle(1.0);
		}

		else if(avg_steering_angle < -10.5){
			IR_setSrvAngle(-1.0);
		}

		else if(avg_steering_angle < 2.7 && avg_steering_angle > -2.7){
			IR_setSrvAngle(0.13);
		}

		else if(avg_steering_angle >= 2.7){//오른쪽 회전
			wheelGain = 0.0989;
			test_angle = wheelGain*avg_steering_angle;
			IR_setSrvAngle(test_angle);
		}

		else{//왼쪽 회전
			wheelGain = 0.0989;
			test_angle = wheelGain*avg_steering_angle;
			IR_setSrvAngle(test_angle);
		}

}
void ControlAngle()
{
	pre_str_ang4 = pre_str_ang3;
	pre_str_ang3 = pre_str_ang2;
	pre_str_ang2 = pre_str_ang1;
	pre_str_ang1 = result_angle;


	angle_Error.error = pre_str_ang1;
	angle_Error.pre_error = pre_str_ang2;
	angle_Error.integral = (pre_str_ang1 + pre_str_ang2 + pre_str_ang3 + pre_str_ang4)/4;
	angle_Error.derivative = angle_Error.error - angle_Error.pre_error;

	switch(g_LineDetc.speedrace_flag){
	case 0:	//speed 1
		angle_Gain.p = 0.98;
		angle_Gain.i = 0.01;
		angle_Gain.d = 0.01;
		break;
	case 1:	//school
		angle_Gain.p = 0.98;
		angle_Gain.i = 0.01;
		angle_Gain.d = 0.01;
		break;
	case 2:	//speed 2
		angle_Gain.p = 0.98;
		angle_Gain.i = 0.01;
		angle_Gain.d = 0.01;
		break;
	default:
		break;
	}
	avg_steering_angle = angle_Gain.p * angle_Error.error + angle_Gain.d * angle_Error.derivative + angle_Error.integral * angle_Gain.i;

}


void PurePursuit()
{
	//midline = 65
	x_point = (56.5-(g_LineDetc.avgLeftLine+g_LineDetc.avgRightLine)/2)*pixel_mm;
	R_dist = (look_ahead_dist*look_ahead_dist + x_point*x_point)/(2*x_point);
	parameter = wheelbase/R_dist;
	result_angle = parameter*(180/IFX_PI);


	//converting discrete values
	//result_angle = 0.6242*exp(result_angle);

	ControlAngle();

	if(avg_steering_angle > 10){
		IR_setSrvAngle(1.0);
	}

	else if(avg_steering_angle < -10){
		IR_setSrvAngle(-1.0);
	}
	else if(avg_steering_angle < 2.45 && avg_steering_angle > -2.45){//2.8~-1.8
		IR_setSrvAngle(0.13);
	}

	else if(avg_steering_angle >= 2.45){//오른쪽 회전
		wheelGain = 0.098;
		test_angle = wheelGain*avg_steering_angle;
		IR_setSrvAngle(test_angle);
	}

	else{//왼쪽 회전
		wheelGain = 0.099;
		test_angle = wheelGain*avg_steering_angle;
		IR_setSrvAngle(test_angle);
	}
}

void FollowOneLine()
{
	switch(g_LineSchoolz.nowPosition){
	/********** go to left line **********/
	case SCHOOLZ_RIGHT :
		FollowRightLine();
		break;
	case SCHOOLZ_LEFT :
		FollowLefttLine();
		break;
	}
}

//void AvoidObstacle()
//{
////   g_LineSchoolz.nowPosition = SCHOOLZ_LEFT;   // test용
//
//   switch(g_LineSchoolz.nowPosition){
//   /********** go to left line **********/
//   case SCHOOLZ_RIGHT :
//      switch(g_LineSchoolz.avoidanceStage){
//         case NO_OBSTACLE:
//            FollowRightLine();
//             break;
//
//         case OBSTACLE_AVOID:
//             IR_setSrvAngle(-1.0);
//             g_LineSchoolz.cntObstacle++;
//             break;
//
//         case OBSTACLE_FOLLOW_LINE:
//            IR_setSrvAngle(-1.0);
//            g_LineSchoolz.cntObstacle++;
//             if(g_LineDetc.avgLeftLine < 69 && g_LineSchoolz.cntObstacle > 70){
//             g_LineSchoolz.avoidanceStage = OBSTACLE_STABILITY;
//             g_LineSchoolz.cntObstacle = 0;
//          }
//
//            break;
//         case OBSTACLE_STABILITY:
//            IR_setSrvAngle(1.0);
//            g_LineSchoolz.cntObstacle++;
//            if(g_LineDetc.avgLeftLine < 65 || g_LineSchoolz.cntObstacle > 30){
//              g_LineSchoolz.cntObstacle = 0;
//              g_LineSchoolz.avoidanceStage = NO_OBSTACLE;
//            g_LineSchoolz.nowPosition = SCHOOLZ_LEFT;
//         }
//            break;
//         default :   // no obstacle
//            break;
//         }
//      FollowRightLine();
//      if(g_PSD.stop_state != 0)//detecting obstacle
//         schz_obstc_flag = 1;
//      if(schz_obstc_flag == 1){
//         IR_setSrvAngle(-1.0);
//         g_LineSchoolz.cntObstacle++;
//         if(g_LineSchoolz.cntObstacle > 100){
//            FollowLeftLine();
//            schz_obstc_flag = 0;
//         }
//      }
//      break;
//
//   /********** go to right line **********/
//   case SCHOOLZ_LEFT :
//      switch(g_LineSchoolz.avoidanceStage){
//         case NO_OBSTACLE:
//            FollowLeftLine();
//             break;
//
//         case OBSTACLE_AVOID:
//             IR_setSrvAngle(1.0);
//             g_LineSchoolz.cntObstacle++;
//             break;
//
//         case OBSTACLE_FOLLOW_LINE:
//            g_LineSchoolz.cntObstacle++;
//            IR_setSrvAngle(1.0);
//             if(g_LineDetc.avgRightLine > 55 && g_LineSchoolz.cntObstacle > 70){
//                g_LineSchoolz.avoidanceStage = OBSTACLE_STABILITY;
//                g_LineSchoolz.cntObstacle = 0;
//             }
//
//             break;
//
//         case OBSTACLE_STABILITY:
//            IR_setSrvAngle(-1.0);
//            g_LineSchoolz.cntObstacle++;
//            if(g_LineDetc.avgLeftLine > 50 || g_LineSchoolz.cntObstacle > 30){
//            g_LineSchoolz.avoidanceStage = NO_OBSTACLE;
//            g_LineSchoolz.nowPosition = SCHOOLZ_RIGHT;
//            g_LineSchoolz.cntObstacle = 0;
//          }
//            break;
//         default :   // no obstacle
//             break;
//         }
//      FollowLeftLine();
//      if(g_PSD.stop_state != 0)//detecting obstacle
//         schz_obstc_flag = 1;
//      if(schz_obstc_flag == 1){
//         IR_setSrvAngle(1.0);
//         g_LineSchoolz.cntObstacle++;
//         if(g_LineSchoolz.cntObstacle > 100){
//            FollowRightLine();
//            schz_obstc_flag = 0;
//         }
//      }
//      break;
//
//   default :
//      break;
//   }



void AvoidObstacle()
{
	g_LineSchoolz.nowPosition = SCHOOLZ_RIGHT;	// test용

	switch(g_LineSchoolz.nowPosition){
	/********** go to left line **********/
	case SCHOOLZ_RIGHT :

		switch(g_LineSchoolz.avoidanceStage){
			case NO_OBSTACLE:
				FollowRightLine();
				break;

		/*	case OBSTACLE_AVOID:
				IR_setSrvAngle(-1.0);
				g_LineSchoolz.cntObstacle++;
				break;

			case 1:
				g_LineSchoolz.cntObstacle++;
				if(g_LineDetc.avgLeftLine < 70 && g_LineDetc.avgLeftLine > 40
						&& g_LineSchoolz.cntObstacle > 60){//1초
					g_LineSchoolz.avoidanceStage = 2;
					g_LineSchoolz.cntObstacle = 0;
				}
				break;
*/
			case 2:
				IR_setSrvAngle(1.0);
				g_LineSchoolz.cntObstacle++;
				if(g_LineDetc.avgLeftLine < 70 && g_LineDetc.avgLeftLine > 40
						&& g_LineSchoolz.cntObstacle > 60){//1초
					g_LineSchoolz.avoidanceStage = NO_OBSTACLE;
					g_LineSchoolz.cntObstacle = 0;
				}
				break;
			default :	// no obstacle
				break;
			}
		break;

	/********** go to right line **********/
	case SCHOOLZ_LEFT :
		switch(g_LineSchoolz.avoidanceStage){
			case NO_OBSTACLE:
				break;

			case OBSTACLE_AVOID:
				IR_setSrvAngle(1.0);
				g_LineSchoolz.cntObstacle++;
				break;

			case OBSTACLE_FOLLOW_LINE:
				if(g_LineDetc.avgRightLine < 110 && g_LineDetc.avgRightLine > 80
						&& g_LineSchoolz.cntObstacle > 50){//1초
					g_LineSchoolz.avoidanceStage = NO_OBSTACLE;
					g_LineSchoolz.cntObstacle = 0;
				}
				break;

			default :	// no obstacle
				break;
			}
		break;

	default :
		break;
	}


}

void HillRun()
{

}

void AEB()
{
	switch(g_AEBterm.riskLevel)
	{
		case 1:
			IR_setMotor0Vol(g_AEBterm.VELOCITY1);
			break;

		case 2:
			IR_setMotor0Vol(g_AEBterm.VELOCITY2);
			break;

		case 3:
			IR_setMotor0Vol(g_AEBterm.VELOCITY3);
			break;

		default:
			//disable motor
			IR_setMotor0Vol(g_AEBterm.STOP);
			break;
	}
}
