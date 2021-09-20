/*
 * Timer0_Prog.c
 *
 *  Created on: May 25, 2019
 *      Author: salsa
 */
#include "std_types.h"
#include "Bit_Math.h"
#include "Timer0_Reg.h"
#include "Timer0_Priv.h"
#include "Timer0_Config.h"
#include "Timer0_Int.h"

static volatile void (* Timer0_INT_ptr) (void);
u8 over_flow_counter =0;

void TIMER_init(void){


#if (OPERATING_MODE == NORMAL_MODE)
CLEAR_BIT(TCCR0,WGM01);
CLEAR_BIT(TCCR0,WGM00);
CLEAR_BIT(TCCR0,COM01);
CLEAR_BIT(TCCR0,COM00);
#elif(OPERATING_MODE == PWM_PHASE_CORRECT_MODE)
CLEAR_BIT(TCCR0,WGM01);
SET_BIT(TCCR0,WGM00);
#if (COMPARE_OUTPUT_PHASE_CORRECT_PWM_MODE == NORMAL_OUTPUT)
CLEAR_BIT(TCCR0,COM01);
CLEAR_BIT(TCCR0,COM00);
#elif(COMPARE_OUTPUT_PHASE_CORRECT_PWM_MODE == SET_OC0_DOWNCOUNT_CLEAR_UPCOUNT)
CLEAR_BIT(TCCR0,COM01);
CLEAR_BIT(TCCR0,COM00);
#elif(COMPARE_OUTPUT_PHASE_CORRECT_PWM_MODE == CLEAR_OC0_DOWNCOUNT_SET_UPCOUNT)
CLEAR_BIT(TCCR0,COM01);
CLEAR_BIT(TCCR0,COM00);
#endif


#elif(OPERATING_MODE == CTC_MODE)
#if(COMPARE_OUTPUT_NO_PWM_MODE == NORMAL_COMPARE_OUTPUT )
CLEAR_BIT(TCCR0,COM01);
CLEAR_BIT(TCCR0,COM00);
#elif(COMPARE_OUTPUT_NO_PWM_MODE == SET_OC0_ON_COMPARE_MATCH )
SET_BIT(TCCR0,COM01);
SET_BIT(TCCR0,COM00);
#elif(COMPARE_OUTPUT_NO_PWM_MODE == CLEAR_OC0_ON_COMPARE_MATCH )
SET_BIT(TCCR0,COM01);
CLEAR_BIT(TCCR0,COM00);
#endif


#elif(OPERATING_MODE == FAST_PWM_MODE)


#if(COMPARE_OUTPUT_FAST_PWM_MODE == NORMAL_OUTPUY)
SET_BIT(TCCR0,COM00);
SET_BIT(TCCR0,COM00);
#elif(COMPARE_OUTPUT_FAST_PWM_MODE == SET_OC0_ON_COMPARE_MATCH )
SET_BIT(TCCR0,COM01);
SET_BIT(TCCR0,COM00);
#elif(COMPARE_OUTPUT_FAST_PWM_MODE == CLEAR_OC0_ON_COMPARE_MATCH )
SET_BIT(TCCR0,COM01);
CLEAR_BIT(TCCR0,COM00);
#endif

#endif

/*NO_CLK
 *CLK_1
 *CLK_8
 *CLK_64
 *CLK_256
 *CLK_1024
 *EXTERNAL_ON_T0_COUNT_ON_FALLING_EDGE
 *EXTERNAL_ON_T0_COUNT_ON_RISING_EDGE
 */
#if(CLOCK_PRESCALER_SELECT == NO_CLK)
CLEAR_BIT(TCCR0,CS00);
CLEAR_BIT(TCCR0,CS01);
CLEAR_BIT(TCCR0,CS02);
#elif(CLOCK_PRESCALER_SELECT == CLK_1)
SET_BIT(TCCR0,CS00);
CLEAR_BIT(TCCR0,CS01);
CLEAR_BIT(TCCR0,CS02);
#elif(CLOCK_PRESCALER_SELECT == CLK_8)
CLEAR_BIT(TCCR0,CS00);
SET_BIT(TCCR0,CS01);
CLEAR_BIT(TCCR0,CS02);
#elif(CLOCK_PRESCALER_SELECT == CLK_64)
SET_BIT(TCCR0,CS00);
SET_BIT(TCCR0,CS01);
CLEAR_BIT(TCCR0,CS02);
#elif(CLOCK_PRESCALER_SELECT == CLK_256)
CLEAR_BIT(TCCR0,CS00);
CLEAR_BIT(TCCR0,CS01);
SET_BIT(TCCR0,CS02);
#elif(CLOCK_PRESCALER_SELECT == CLK_1024)
SET_BIT(TCCR0,CS00);
CLEAR_BIT(TCCR0,CS01);
SET_BIT(TCCR0,CS02);
#elif(CLOCK_PRESCALER_SELECT == EXTERNAL_ON_T0_COUNT_ON_FALLING_EDGE)
CLEAR_BIT(TCCR0,CS00);
SET_BIT(TCCR0,CS01);
SET_BIT(TCCR0,CS02);
#elif(CLOCK_PRESCALER_SELECT == EXTERNAL_ON_T0_COUNT_ON_RISING_EDGE)
SET_BIT(TCCR0,CS00);
SET_BIT(TCCR0,CS01);
SET_BIT(TCCR0,CS02);

#endif

}
void TIMER_init(void){
	TCNT0 = 0x00;
}
void TIMER_voidForceOutPutCompareMode(void){
	SET_BIT(TCCR0,FOC0);
}
void TIMER_voidEnableOutputCompareInterrupt(void){
SET_BIT(TIMSK,OCIE0);
}
void TIMER_voidEnableOverFlowInterrupt(void){
	SET_BIT(TIMSK,TOIE0);
}
void TIMER_WriteInTCNT0(u8 time){
	TCNT0 = time;
}
void TIMER_WriteInOCR0(u8 value){
	OCR0 = value;
}

#if(OPERATING_MODE == NORMAL_MODE)

void TIMER_SetCallBack(void (* TimerInterrupt_ptr)(void)){
	Timer0_INT_ptr = TimerInterrupt_ptr;
}

void __vector_11(void)__attribute((signal,used));
void __vector_11(void) {
	Timer0_INT_ptr();
}
#endif
#if(OPERATING_MODE == CTC_MODE)
void Timer0_SetCallBack(void (* CaptureCompareInterrupt_ptr)(void)){
	Timer0_INT_ptr = CaptureCompareInterrupt_ptr;
}
void __vector_10(void)__attribute((signal,used));
void __vector_10(void) {
	Timer0_INT_ptr();
}
#endif
