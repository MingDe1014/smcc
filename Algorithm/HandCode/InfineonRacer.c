/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "InfineonRacer.h"
#include "Basic.h"

//#define MODE1_ON 0
//#define MODE1_OFF 1
//#define MODE2_ON 0
//#define MODE2_OFF 1
//
//#define SPEEDRACE 0.7
//#define SAFERACE 0.3

static sint32 u_task_cnt_5m = 0;
static sint32 u_task_cnt_10m = 0;
//static sint32 user_cnt_5m = 0;

uint8 stage1_flag;
uint8 stage2_flag;
uint8 schz_check2_flag;

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
void ProgramRun_5ms(void){
	u_task_cnt_5m++;
	if(u_task_cnt_5m == 1000){
		u_task_cnt_5m = 0;
	}


	stage1_flag = Mode_1();
	stage2_flag = Mode_2();

	BasicPort_run();
	BasicVadcBgScan_run();

	//예선
	if(stage1_flag == MODE1_ON && stage2_flag ==MODE2_OFF){
//		switch(g_PSD.stop_state){
//		case 0:
		if(u_task_cnt_5m%2 == 0){
			Stage1_run();
		}

//			break;
//
//		default: //end of game
			if(g_PSD.stop_state != 0){
				IR_setMotor0Vol(0.0);
				IR_setSrvAngle(0.13);
			}
//			break;
//		}

		BasicGtmTom_run();
	}

	//본선
	else if(stage1_flag == MODE1_OFF && stage2_flag == MODE2_ON){
		if(u_task_cnt_5m%2 == 0){
			LineScan10ms();
			PurePursuit();
		}
		Stage2_run();
		BasicGtmTom_run();
	}
}

void LineScan10ms(void){
	u_task_cnt_10m++;
	if(u_task_cnt_10m == 1000){
		u_task_cnt_10m = 0;
	}

	BasicLineScan_run();
	uNorm();
	uLineFilter();
	avgLane();
}

void Stage1_run(void)
{
	LineScan10ms();
	PurePursuit();
	IR_setMotor0Vol(0.2);
}

void Stage2_run(void)
{
//	switch(g_LineDetc.speedrace_flag)
//	{
//		case 0://first speed race
//			IR_setMotor0Vol(0.3);
//			Schoolz_check1();
//			break;
//
//		case 1://school zone race
			g_LineDetc.speedrace_flag = 2;
//			IR_setMotor0Vol(0.23);
//			user_cnt_5m++;
//
//			/************************/
//			if(user_cnt_5m == 400){//2sec
//				schz_check2_flag = 1;
//				user_cnt_5m = 0;
//			}

//			if(schz_check2_flag == 1){
//
//				//Schoolz_check2();
//				FollowOneLine();
//				//Schoolz_checkPosition();
//				AvoidObstacle();
//			}
//			/************************/

//			break;
//
		//case 2://second speed race

			if(g_AEBterm.riskLevel == 0){
				IR_setMotor0Vol(SPEEDRACE);
			}
			else{
				//after hill detect
				if(g_PP.hillDetc == 1 && g_PP.hillEnd == 1){
					AEB();
				}
				else{// deceleration before hill
					//HillRun();
					IR_setMotor0Vol(SPEEDRACE * 0.8);
				}
			}
			//break;
//
//		default:
//			break;
//	}
}

