#include "AppTaskFu.h"

static sint32 task_cnt_1m = 0;
static sint32 task_cnt_10m = 0;
static sint32 task_cnt_100m = 0;
static sint32 task_cnt_1000m = 0;

//static sint32 task_cnt_20m = 0;

boolean task_flag_1m = FALSE;
boolean task_flag_10m = FALSE;
boolean task_flag_100m = FALSE;
boolean task_flag_1000m = FALSE;

//uint8 stage1_flag = 0;
//uint8 final_stage_flag = 0;
//uint8 schz2_flag = 0;
//uint8 obstacle_detec = 0;

void appTaskfu_init(void){
	BasicLineScan_init();
	BasicPort_init();
    BasicGtmTom_init();
    BasicVadcBgScan_init();
    BasicGpt12Enc_init();
    AsclinShellInterface_init();
    IR_setMotor0En(1);

#if BOARD == APPLICATION_KIT_TC237
    tft_app_init(1);
    perf_meas_init();
#elif BOARD == SHIELD_BUDDY

#endif

#if CODE == CODE_HAND
    //InfineonRacer_init();
#elif CODE == CODE_ERT
    IR_Controller_initialize();
#else

#endif
}

void appTaskfu_1ms(void)
{
	task_cnt_1m++;
	if(task_cnt_1m == 1000){
		task_cnt_1m = 0;
	}

	if(task_cnt_1m%5 == 0){
		ProgramRun_5ms();
	}




//	if(task_cnt_1m%2 == 0){
//		//Yaemin_2ms();
//		IR_setMotor0Vol(0.17);
//		BasicPort_run();
//		GtmTomPwmHl_run();
//	}

	//if(task_cnt_1m%2 == 0){	// psd, servo 움직임

/*
		if(g_PSD.stop_state == 0){
			stage1_flag = Mode_1();
			final_stage_flag = Mode_2();
			if(stage1_flag == MODE1_ON){
				obstacle_detec = 0;
			}
		}
		//switch & motor
		BasicPort_run();
		BasicVadcBgScan_run();

		//예선

		if(final_stage_flag == MODE2_OFF){
			switch(stage1_flag){
			case MODE1_ON:	//예선 주행 모드 on
				if(obstacle_detec == 0){
					IR_setMotor0Vol(0.2);
					switch(g_PSD.stop_state){
					case 0:
						break;

					default:
						AEB();
						obstacle_detec = 1;
						break;
					}
					BasicGtmTom_run();
				}
				break;
				default:
					break;
			}
		}*/
			/********************************************************/
					//본선
				/*	if(stage1_flag == MODE1_OFF){
						switch(final_stage_flag){
						case MODE2_OFF:
							break;

						//본선 주행 모드 on
						case MODE2_ON:
							switch(g_LineDetc.speedrace_flag){
							//first speed racing
							case 0:
								Schoolz_check1();
								IR_setMotor0Vol(SPEEDRACE_VELOCITY);
								break;

							//school zone
							case 1:
								Schoolz_check2();
								IR_setMotor0Vol(SCHOOLZ_VELOCITY);
								break;

								//second speed racing
							case 2:
								if(g_PSD.stop_state == 0){
									IR_setMotor0Vol(SPEEDRACE_VELOCITY);
								}
								else{
									//after hill detect
									if(g_PP.hillDetc == 1){
										AEB();
									}
									else{
										HillRun();
									}
								}

								break;
							}
							BasicGtmTom_run();
							break;

						default:
							break;
						}
					}*/
			/********************************************************/

	//}
}


void appTaskfu_10ms(void)
{
	task_cnt_10m++;
	if(task_cnt_10m == 1000){
		task_cnt_10m = 0;
	}


//	if(task_cnt_10m%2 == 0){
//
//		BasicLineScan_run();
//		uNorm();
//	}



//
//	IR_setLed0(0);
//	BasicPort_run();
//	if(g_PSD.stop_state == 0){
//		stage1_flag = Mode_1();
//		final_stage_flag = Mode_2();
//		if(final_stage_flag == MODE2_ON && stage1_flag == MODE1_OFF){
//			obstacle_detec = 0;
//		}
//		if(stage1_flag == MODE1_ON && final_stage_flag == MODE2_OFF){
//			obstacle_detec = 0;
//		}
//	}
//	//switch & motor
//	BasicPort_run();
//	BasicVadcBgScan_run();
//
//	BasicLineScan_run();
//	uNorm();
//	uLineFilter();
//	avgLane();
//
//	//예선
//	if(final_stage_flag == MODE2_OFF){
//		switch(stage1_flag){
//		case MODE1_ON:	//예선 주행 모드 on
//			if(obstacle_detec == 0){
//				switch(g_PSD.stop_state){
//				case 0:
//					switch(g_LineDetc.speedrace_flag){
//					case 0:
//						PurePursuit();
//						IR_setMotor0Vol(0.30);
//						Schoolz_check1();
//						break;
//					case 1:
//						task_cnt_20m++;
//						IR_setMotor0Vol(0.20);
//
//						/********************************************************/
//						//school zone 진입 후 2초 뒤에 다시 school zone 빠져나가는 함수 인식 시작
//						if(task_cnt_20m == 100){
//							schz2_flag = 1;
//						}
//						if(schz2_flag == 1){
//							Schoolz_check2();
//						}
//						/********************************************************/
//						break;
//					case 2:
//						PurePursuit();
//						IR_setMotor0Vol(0.30);
//						break;
//					default:
//						break;
//					}
//					break;
//
//				default:
//					AEB();
//					obstacle_detec = 1;
//					break;
//				}
//			break;
//			default:
//				IR_setSrvAngle(0.13);
//				IR_setMotor0Vol(0.0);
//				break;
//			}
//		}
//	}
//	/************************************************************************/
//	//본선
//	if(final_stage_flag == MODE1_OFF){
//		switch(final_stage_flag){
//		case MODE2_ON:
//			if(obstacle_detec == 0){
//				switch(g_PSD.stop_state){
//				case 0:
//					switch(g_LineDetc.speedrace_flag){
//					case 0://first speed racing
//						PurePursuit();
//						IR_setMotor0Vol(SPEEDRACE_VELOCITY);
//						Schoolz_check1();
//						break;
//
//					case 1://school zone
//						task_cnt_20m++;
//						IR_setMotor0Vol(SCHOOLZ_VELOCITY);
//
//						/********************************************************/
//						//school zone 진입 후 2초 뒤에 다시 school zone 빠져나가는 함수 인식 시작
//						if(task_cnt_20m == 100){
//							schz2_flag = 1;
//						}
//						if(schz2_flag == 1){
//							Schoolz_check2();
//						}
//						/********************************************************/
//						break;
//
//					case 2://speed speed racing
//						PurePursuit();
//						IR_setMotor0Vol(SPEEDRACE_VELOCITY);
//						break;
//
//					default:
//						break;
//					}
//					break;
//				default:	//자율 비상제동 함수 위치
//					AEB();
//					obstacle_detec = 1;
//					break;
//				}
//
//			}
//			break;
//
//		case MODE2_OFF:
//			IR_setSrvAngle(0.13);
//			IR_setMotor0Vol(0.0);
//			break;
//
//		default:
//			break;
//		}
//	}
//	BasicGtmTom_run();
	/************************************************************************/

//	//20ms
//	if(task_cnt_10m%2 == 0){
//		if(task_cnt_20m == 1000){
//			task_cnt_20m = 0;
//		}
//
//		if(g_PSD.stop_state == 0){
//			stage1_flag = Mode_1();
//			final_stage_flag = Mode_2();
//			if(stage1_flag == MODE1_ON){
//				obstacle_detec = 0;
//			}
//		}
//		//switch & motor
//		BasicPort_run();
//		BasicVadcBgScan_run();
//
//		BasicLineScan_run();
//		uNorm();
//		uLineFilter();
//		avgLane();
//
//		//예선
//
//		if(final_stage_flag == MODE2_OFF){
//			switch(stage1_flag){
//			case MODE1_ON:	//예선 주행 모드 on
//				if(obstacle_detec == 0){
//					switch(g_PSD.stop_state){
//					case 0:
//						switch(g_PP.LinePosition){
//						case 0:	//initial value
//							PurePursuit();
//							break;
//						case 1: //left
//							FollowLeftLine();
//							break;
//						case 2: //right
//							FollowRightLine();
//							break;
//						default:
//							break;
//
//						}
						//PurePursuit();
						//FollowLeftLine();
//						switch(g_LineDetc.speedrace_flag){
//						case 0:
//							PurePursuit();
//							IR_setMotor0Vol(0.40);
//							Schoolz_check1();
//							break;
//						case 1:
//							task_cnt_20m++;
//							IR_setMotor0Vol(0.30);
//
//							/********************************************************/
//							//school zone 진입 후 2초 뒤에 다시 school zone 빠져나가는 함수 인식 시작
//							if(task_cnt_20m == 100){
//								schz2_flag = 1;
//							}
//							if(schz2_flag == 1){
//								Schoolz_check2();
//							}
//							/********************************************************/
//							break;
//						case 2:
//							PurePursuit();
//							IR_setMotor0Vol(0.40);
//							break;
//						default:
//							break;
//						}
//						break;
//
//					default:
//						AEB();
//						obstacle_detec = 1;
//						break;
//					}
//					BasicGtmTom_run();
//				break;
//				default:
//					break;
//			}
//		}



	/*
		if(IR_Ctrl.basicTest == FALSE){
			#if CODE == CODE_HAND
				InfineonRacer_control();
			#elif CODE == CODE_ERT
				IR_Controller_step();
			#else

			#endif
		}

		AsclinShellInterface_runLineScan();
	*/
}

void appTaskfu_100ms(void)
{
	task_cnt_100m++;
	if(task_cnt_100m == 1000){
		task_cnt_100m = 0;
	}
#if BOARD == APPLICATION_KIT_TC237
	if(task_cnt_100m % REFRESH_TFT == 0){
		tft_app_run();
	}

#elif BOARD == SHIELD_BUDDY

#endif
}

void appTaskfu_1000ms(void)
{
	task_cnt_1000m++;

	if(task_cnt_1000m == 1000){
		task_cnt_1000m = 0;
	}
}

void appTaskfu_idle(void){
	AsclinShellInterface_run();
#if BOARD == APPLICATION_KIT_TC237
	perf_meas_idle();
#elif BOARD == SHIELD_BUDDY

#endif

}

void appIsrCb_1ms(void){
	BasicGpt12Enc_run(); // 엔코더 안쓰니깐 일단 주석처리해놓음
}

