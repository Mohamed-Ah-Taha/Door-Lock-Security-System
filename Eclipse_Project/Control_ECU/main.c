/*
 * main.c
 *
 *  Created on: Jun 24, 2021
 *      Author: Mohamed Taha
 */

#define F_CPU 8000000
#include "buzzer.h"
#include "DC_motor.h"
#include "EEPROM.h"
#include "i2c.h"
#include "timer.h"
#include "uart.h"

#define M2_READY 0x10
#define M1_READY 0x11
/* Commands that will be received from HMI MC */
#define COMMAND_1 0x40
#define COMMAND_2 0x41
#define COMMAND_3 0x42
#define COMMAND_4 0x43
#define COMMAND_5 0x44

#define APP_IS_INITIALIZED 0xFA
#define APP_IS_NOT_INITIALIZED 0xF5

#define TIMER_15_SECS 457
#define TIMER_60_SECS 1831

#define E2PROM_FIRST_TIME 0x25

#define BUZZER_FINISH_FLAG 0xF8
#define MOTOR_FINISH_FLAG 0xF9


/* Global Variables */
uint16 tick_buzzer = 0;
uint16 tick_motor = 0;
uint8 g_flag_buzzer=0;
uint8 g_flag_motor=0;
uint8 g_password_array[5] = {0};
/* Create configuration structure for Timer driver */
Timer_ConfigType Timer_Config = {TIMER_0,NORMAL,0,0,F_CPU_1024};

/* Function Prototypes */
void Timer0_Callbackfunc(void);
void check_first_time(void);
void write_first_time(void);
void send_pass(void);
void buzzer_on(void);
void motor_on(void);

int main(void)
{
	/* Initialization */
	/*******************
	 *     Variables   *
	 *******************/
	uint8 receivedByte = 0 ; /* variable used to store data received from UART */

	/*******************
	 *     DC Motor    *
	 *******************/
	DCMotor_init();
	/*******************
	 *       I2C       *
	 *******************/
	TWI_ConfigType TWI_Config = {RATE2,0x05};
	TWI_init(&TWI_Config);

	/*******************
	 *    	 UART      *
	 *******************/
	/* Create configuration structure for UART driver */
	UART_ConfigType UART_Config = {EIGHT_Bit,Disabled,ONE_Bit,9600};
	UART_init(&UART_Config);
	while(UART_recieveByte() != M1_READY){} // wait until MC1 is ready
	UART_sendByte(M2_READY); // MC2 is ready
	/*******************
	 *    	TIMER      *
	 *******************/
	Timer0_setCallBack(Timer0_Callbackfunc);

	while(1)
	{
		/* Application */
		/* The HMI MC will be sending commands to the Control MC through UART */
		/*
		 * Command 1 : 0x40 -> Check if its the first time to run the app or no ( if there is a password saved or no)
		 * Command 2 : 0x41 -> If its the first time , write the new password to the E2PROM
		 * Command 3 : 0x42 -> Sends the password the MC in case its not the first time
		 * Command 4 : 0x43 -> Turns on the buzzer in case the user wrote the wrong password
		 * Command 5 : 0x44 -> Turns on the door(motor) in case the user wrote the right password
		 * */
		receivedByte = UART_recieveByte();
		if(receivedByte == COMMAND_1)
		{
			check_first_time();
		}
		if(receivedByte == COMMAND_2)
		{
			write_first_time();
		}
		if(receivedByte == COMMAND_3)
		{
			send_pass();
		}
		if(receivedByte == COMMAND_4)
		{
			buzzer_on();
		}
		if(receivedByte == COMMAND_5)
		{
			motor_on();
		}
	}
	return 0;
}

void Timer0_Callbackfunc(void)
{
	/* We are using F_CPU = 8MHz and prescaler = 1024 so one second is 30 ticks */
	tick_buzzer ++ ;
	tick_motor ++ ;
	if(tick_motor == TIMER_15_SECS) /* for counting 15 seconds */
	{
		g_flag_motor = 1;
		tick_motor = 0;
	}
	if(tick_buzzer == TIMER_60_SECS) /* for counting 60 seconds */
	{
		g_flag_buzzer = 1;
		tick_buzzer = 0;
	}
}

void check_first_time(void)
{
	uint8 EEPROM_val = 0; //variable used to store data from EE2PROM
	EEPROM_readByte(0x0300, &EEPROM_val);
	if(EEPROM_val == E2PROM_FIRST_TIME) /* 0x25 means i wrote there before */
	{
		UART_sendByte(APP_IS_INITIALIZED); /* there is value inside */
	}
	else
	{
		UART_sendByte(APP_IS_NOT_INITIALIZED); /* there is no value inside */
	}
}

void write_first_time(void)
{
	uint8 receivedByte;
	EEPROM_writeByte(0x0300, E2PROM_FIRST_TIME);
	_delay_ms(10);
	for(int i = 0 ; i < 5 ; i++)
	{
		receivedByte = UART_recieveByte();
		g_password_array[i] = receivedByte;
		UART_sendByte(M2_READY);
	}
	for(int i = 0 ; i < 5 ; i++)
	{
		EEPROM_writeByte(0x0301+i, g_password_array[i]);
		_delay_ms(10);
	}
}

void send_pass(void)
{
	_delay_ms(1000);
	for(int i = 0 ; i < 5 ; i++)
	{
		EEPROM_readByte((0x0301+i), &g_password_array[i]);
		_delay_ms(10);
	}
	for(int i = 0 ; i < 5 ; i++)
	{
		UART_sendByte(g_password_array[i]);
		while(UART_recieveByte() != M1_READY){} // wait until MC2 is ready
	}
}

void buzzer_on(void)
{
	SREG |= (1<<7);
	g_flag_buzzer = 0; /* Clear flag for next loop */
	Buzzer_ON();
	Timer_init(&Timer_Config);
	while(g_flag_buzzer != 1); /* wait till flag is set */
	g_flag_buzzer = 0; /* Clear flag for next loop */
	Buzzer_OFF();
	UART_sendByte(BUZZER_FINISH_FLAG);
	SREG &= ~(1<<7);
}

void motor_on(void)
{
	SREG |= (1<<7);
	g_flag_motor = 0; /* Clear flag */
	DCMotor_clockwise();
	Timer_init(&Timer_Config);
	while(g_flag_motor != 1); /* wait till flag is set */
	g_flag_motor = 0; /* Clear flag for next loop */

	DCMotor_anti_clockwise();
	Timer_init(&Timer_Config);
	while(g_flag_motor != 1); /* wait till flag is set */
	g_flag_motor = 0; /* Clear flag for next loop */
	DCMotor_stop();
	UART_sendByte(MOTOR_FINISH_FLAG);
	SREG &= ~(1<<7);
}
