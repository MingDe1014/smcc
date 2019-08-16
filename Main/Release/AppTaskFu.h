#ifndef APPTASKFU_H_
#define APPTASKFU_H_

#include <Ifx_Types.h>

#include "Basic.h"
#include "AsclinShellInterface.h"
#include "InfineonRacer.h"
#include "IR_Controller.h"

//#define MODE1_ON 0
//#define MODE1_OFF 1
//#define MODE2_ON 0
//#define MODE2_OFF 1
//
//#define SCHOOLZ_VELOCITY 0.3
#define SPEEDRACE_VELOCITY 0.35

#if BOARD == APPLICATION_KIT_TC237
#include "tft_app.h"
#include "Perf_Meas.h"
#elif BOARD == SHIELD_BUDDY

#endif

IFX_EXTERN boolean task_flag_1m;
IFX_EXTERN boolean task_flag_10m;
IFX_EXTERN boolean task_flag_100m;
IFX_EXTERN boolean task_flag_1000m;

void appTaskfu_init(void);
void appTaskfu_1ms(void);
void appTaskfu_10ms(void);
void appTaskfu_100ms(void);
void appTaskfu_1000ms(void);
void appTaskfu_idle(void);
void appIsrCb_1ms(void);

#endif /* APPTASKFU_H_ */
