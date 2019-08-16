#ifndef INFINEONRACER_H_
#define INFINEONRACER_H_


#include <Ifx_Types.h>
#include "Configuration.h"
#include "Basic.h"

#define IR_getLs0Margin()		IR_Ctrl.Ls0Margin
#define IR_getLs1Margin()		IR_Ctrl.Ls1Margin


#define MODE1_ON 0
#define MODE1_OFF 1
#define MODE2_ON 0
#define MODE2_OFF 1

#define SPEEDRACE 0.35
#define SAFERACE 0.24

typedef struct{
	sint32 Ls0Margin;
	sint32 Ls1Margin;
	boolean basicTest;
}InfineonRacer_t;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN InfineonRacer_t IR_Ctrl;

IFX_EXTERN void InfineonRacer_init(void);
IFX_EXTERN void InfineonRacer_detectLane(void);
IFX_EXTERN void InfineonRacer_control(void);

IFX_EXTERN void ProgramRun_5ms(void);
IFX_EXTERN void LineScan10ms(void);
IFX_EXTERN void Stage1_run(void);
IFX_EXTERN void Stage2_run(void);

#endif
