/*
 * timer.c
 *
 *  Created on: Jun 21, 2021
 *      Author: Mohamed Taha
 */


/* Include Module.h file */
#include "timer.h"

/*******************************************************************************
 *       				  		Global Variables   			 		  		   *
 *******************************************************************************/

/* Public (Shared) Global Variables */

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtrTimer0)(void) = NULL_PTR;
static volatile void (*g_callBackPtrTimer1)(void) = NULL_PTR;
static volatile void (*g_callBackPtrTimer2)(void) = NULL_PTR;

/* Private (Static) Global Variables */

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/* Timer0 Overflow ISR */
ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the overflow is detected */
		(*g_callBackPtrTimer0)(); /* another method to call the function using pointer to function g_callBackPtrTimer0(); */
	}
	TIFR  |= (1<<TOV0); // Clear MIF (Interrupt Flag)
}
/* Timer0 Compare ISR */
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match is detected */
		(*g_callBackPtrTimer0)(); /* another method to call the function using pointer to function g_callBackPtrTimer0(); */
	}
	TIFR  |= (1<<OCF0); // Clear MIF (Interrupt Flag)
}
/* Timer1 Overflow ISR */
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the overflow is detected */
		(*g_callBackPtrTimer1)(); /* another method to call the function using pointer to function g_callBackPtrTimer1(); */
	}
	TIFR  |= (1<<TOIE1); // Clear MIF (Interrupt Flag)
}
/* Timer1 Compare ISR */
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match is detected */
		(*g_callBackPtrTimer1)(); /* another method to call the function using pointer to function g_callBackPtrTimer1(); */
	}
	TIFR  |= (1<<OCIE1A); // Clear MIF (Interrupt Flag)
}

/* Timer2 Overflow ISR */
ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the overflow is detected */
		(*g_callBackPtrTimer2)(); /* another method to call the function using pointer to function g_callBackPtrTimer2(); */
	}
	TIFR  |= (1<<TOV2); // Clear MIF (Interrupt Flag)
}
/* Timer2 Compare ISR */
ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match is detected */
		(*g_callBackPtrTimer2)(); /* another method to call the function using pointer to function g_callBackPtrTimer2(); */
	}
	TIFR  |= (1<<OCF2); // Clear MIF (Interrupt Flag)
}
/*******************************************************************************
 *       				Functions Prototypes(Private)  			 		  	   *
 *******************************************************************************/




/*******************************************************************************
 *       				Functions Definitions		  			 		  	   *
 *******************************************************************************/

/* Public (Shared) Functions */
/*
 * Description : Function to initialize the Timer driver
 * 	1. Set the required Channel (Timer0 , Timer1 , Timer2).
 * 	2. Set the required Mode (Normal , PWM , Compare).
 * 	3. Set the Initial Value (Normal or Compare modes).
 * 	4. Set the Compare Value (Compare mode ONLY).
 * 	5. Set Pre_scaler for the timer.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	/* Check on the Timer channel */
	if((Config_Ptr->Channel) == TIMER_0) /*Timer 0*/
	{
		if((Config_Ptr->Mode) == NORMAL)
		{
			TCNT0  = (uint8)(Config_Ptr->Initial); // Timer Counter Register
			/* Configure timer control register
			 * 1. Normal mode FOC0=1
			 * 2. Normal Mode WGM01=0 & WGM00=0
			 * 3. Set Pre-scaler
			 */
			TCCR0  = (1<<FOC0) | (TCCR0 & 0xF8) | (Config_Ptr->Prescaler);  /*For setting Pre-scaler in Normal Mode */
			TIMSK |= (1<<TOIE0); /* Enable Timer0 OVF Interrupt enable (MIE) */
		}
		else if((Config_Ptr->Mode) == COMPARE)
		{
			TCNT0  = (uint8)(Config_Ptr->Initial); // Timer Counter Register
			OCR0   = (uint8)(Config_Ptr->Compare); // Output Compare Register
			/* Configure timer control register
			 * 1. Compare mode FOC0=1
			 * 2. Compare Mode WGM01=1 & WGM00=0
			 * 3. Set Pre-scaler
			 */
			TCCR0  = (1<<FOC0) | (1<<WGM01) | (TCCR0 & 0xF8) | (Config_Ptr->Prescaler);  /*For setting Pre-scaler in Compare Mode */
			TIMSK |= (1<<OCIE0); /* Enable Timer0 Compare Interrupt enable (MIE) */
		}
		else if((Config_Ptr->Mode) == PWM)
		{
			TCNT0  = (uint8)(Config_Ptr->Initial); // Timer Counter Register
			OCR0   = (uint8)(Config_Ptr->Compare); // Output Compare Register
			DDRB  |= (1<<PB3); //set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.
			TCCR0  = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (TCCR0 & 0xF8) | (Config_Ptr->Prescaler);
		}
	}
	else if((Config_Ptr->Channel) == TIMER_1)
	{
		if((Config_Ptr->Mode) == NORMAL)
		{
			TCNT1  = (Config_Ptr->Initial); // Timer Counter Register
			/* Configure timer control register TCCR1A
			 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
			 * 2. FOC1A=1 FOC1B=0
			 * 3. Normal Mode WGM10=0 WGM11=0 (Mode Number 0)
			 */
			TCCR1A = (1<<FOC1A);
			/* Configure timer control register TCCR1B
			 * 1. Normal Mode WGM12=0 WGM13=0 (Mode Number 0)
			 * 2. Set Pre-scaler
			 */
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->Prescaler);
			TIMSK |= (1<<TOIE1); /* Enable Timer1 OVF Interrupt enable (MIE) */
		}
		else if((Config_Ptr->Mode) == COMPARE)
		{
			TCNT1  = (Config_Ptr->Initial); // Timer Counter Register
			OCR1A  = (Config_Ptr->Compare); // Output Compare Register
			/* Configure timer control register TCCR1A
			 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
			 * 2. FOC1A=1 FOC1B=0
			 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
			 */
			TCCR1A = (1<<FOC1A);

			/* Configure timer control register TCCR1B
			 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
			 * 2. Set Pre-scaler
			 */
			TCCR1B = (1<<WGM12) | (TCCR1B & 0xF8) | (Config_Ptr->Prescaler);
			TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare Interrupt enable (MIE)  */
		}
		else if((Config_Ptr->Mode) == PWM)
		{
			TCNT1  = (Config_Ptr->Initial); // Timer Counter Register
			OCR1A  = (Config_Ptr->Compare); // Output Compare Register
			DDRD |= (1<<PD5); //set PD5/OC1A as output pin --> pin where the PWM signal is generated from MC.
			/* Configure timer control register TCCR1A
			 * 1. Connect OC1A and Disconnect OC1B  COM1A1=1 COM1A0=0 COM1B0=0 COM1B1=0
			 * 2. FOC1A=0 FOC1B=0
			 * 3. Fast PWM Mode WGM10=1 WGM11=1 (Mode Number 15)
			 */
			TCCR1A = (1<<COM1A1) | (1<<WGM10) | (1<<WGM11);

			/* Configure timer control register TCCR1B
			 * 1. Fast PW WGM12=1 WGM13=1 (Mode Number 15)
			 * 2. Set Pre-scaler
			 */
			TCCR1B = (1<<WGM12) | (1<<WGM13) | (TCCR1B & 0xF8) | (Config_Ptr->Prescaler);
		}
	}
	else if((Config_Ptr->Channel) == TIMER_2)
	{
		if((Config_Ptr->Mode) == NORMAL)
		{
			TCNT2  = (uint8)(Config_Ptr->Initial); // Timer Counter Register
			/* Configure timer control register
			 * 1. Normal mode FOC2=1
			 * 2. Normal Mode WGM21=0 & WGM20=0
			 * 3. Set Pre-scaler
			 */
			TCCR2  = (1<<FOC2) | (TCCR2 & 0xF8) | (Config_Ptr->Prescaler);  /*For setting Pre-scaler in Normal Mode */
			TIMSK |= (1<<TOIE2); /* Enable Timer0 OVF Interrupt enable (MIE) */
		}
		else if((Config_Ptr->Mode) == COMPARE)
		{
			TCNT2  = (uint8)(Config_Ptr->Initial); // Timer Counter Register
			OCR2   = (uint8)(Config_Ptr->Compare); // Output Compare Register
			/* Configure timer control register
			 * 1. Compare mode FOC2=1
			 * 2. Compare Mode WGM21=1 & WGM20=0
			 * 3. Set Pre-scaler
			 */
			TCCR2  = (1<<FOC2) | (1<<WGM21) | (TCCR2 & 0xF8) | (Config_Ptr->Prescaler);  /*For setting Pre-scaler in Compare Mode */
			TIMSK |= (1<<OCIE2); /* Enable Timer0 Compare Interrupt enable (MIE) */
		}
		else if((Config_Ptr->Mode) == PWM)
		{
			TCNT2 = (uint8)(Config_Ptr->Initial); // Timer Counter Register
			OCR2  = (uint8)(Config_Ptr->Compare); // Output Compare Register
			DDRD |= (1<<PD7); //set PD7/OC2 as output pin --> pin where the PWM signal is generated from MC.
			TCCR2 = (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (TCCR2 & 0xF8) | (Config_Ptr->Prescaler);
		}
	}
}

/*
 * Description: Function to set the Call Back function address for timer0 .
 */
void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtrTimer0 = a_ptr;
}

/*
 * Description: Function to set the Call Back function address for timer1 .
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtrTimer1 = a_ptr;
}

/*
 * Description: Function to set the Call Back function address for timer2 .
 */
void Timer2_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtrTimer2 = a_ptr;
}

/*
 * Description: Function to disable the Timer
 */
void Timer_DeInit(const Timer_channel Channel)
{
	if((Channel) == TIMER_0)
	{
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0  = 0;
		TIMSK &= ~(1<<TOIE0) & ~(1<<OCIE0);
	}
	else if((Channel)== TIMER_1)
	{
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;
		TIMSK &= ~(1<<TOIE1) & ~(1<<OCIE1A);
	}
	else if((Channel) == TIMER_2)
	{
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2  = 0;
		TIMSK &= ~(1<<TOIE2) & ~(1<<OCIE2);
	}
}

/* Function to set PWM , it can work with timer0 or timer2 , duty cycle is a value from 0 -> 100 (%) */
void PWM_setDutyCycle(uint16 OCR_value, const Timer_channel Channel)
{
	if((Channel) == TIMER_0)
	{
		OCR0 = (uint8)OCR_value;
	}
	else if((Channel)== TIMER_1)
	{
		OCR1A  = OCR_value;
	}
	else if((Channel) == TIMER_2)
	{
		OCR2 = (uint8)OCR_value;
	}
}
/* Private (Static) Functions */

