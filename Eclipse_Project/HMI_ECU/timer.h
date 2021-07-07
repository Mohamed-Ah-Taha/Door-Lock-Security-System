/*
 * timer.h
 *
 *  Created on: Jun 21, 2021
 *      Author: Mohamed Taha
 */

#ifndef TIMER_H_
#define TIMER_H_

/* Include Common header files */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*******************************************************************************
 *         Preprocessor Macros (Definitions and static Configurations)         *
 *******************************************************************************/


/*******************************************************************************
 *       					  User-defined Types      						   *
 *******************************************************************************/
typedef enum
{
	TIMER_0 , TIMER_1 ,TIMER_2
}Timer_channel;

typedef enum
{
	NORMAL,COMPARE=2,PWM=3
}Timer_Mode;

typedef uint16 Timer_Initial_value;

/* Only in compare mode, if not used initialize it with 0 */
typedef uint16 Timer_Compare_value;

typedef enum
{
	NO_CLOCK , F_CPU_CLOCK , F_CPU_8 , F_CPU_64 , F_CPU_256 , F_CPU_1024 , EXT_CLK_falling , EXT_CLK_rising
}Timer_Prescaler;

typedef struct
{
	Timer_channel Channel;
	Timer_Mode Mode;
	Timer_Initial_value Initial;
	Timer_Compare_value Compare;
	Timer_Prescaler Prescaler;

}Timer_ConfigType;

/*******************************************************************************
 *       				  Extern for Public Global Variables   		 		   *
 *******************************************************************************/


/*******************************************************************************
 *       					  Functions Prototypes 	   				 		   *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer driver
 * 	1. Set the required Channel (Timer0 , Timer1 , Timer2).
 * 	2. Set the required Mode (Normal , PWM , Compare).
 * 	3. Set the Initial Value (Normal or Compare modes).
 * 	4. Set the Compare Value (Compare mode ONLY).
 * 	5. Set Pre_scaler for the timer.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address for timer0 .
 */
void Timer0_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to set the Call Back function address for timer1 .
 */
void Timer1_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to set the Call Back function address for timer2 .
 */
void Timer2_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to disable the Timer
 */
void Timer_DeInit(const Timer_channel Channel);

/*
 * Description: Function to change PWM value
 */
void PWM_setDutyCycle(uint16 OCR_value, const Timer_channel Channel);

#endif /* TIMER_H_ */
