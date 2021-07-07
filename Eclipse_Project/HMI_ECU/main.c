/*
 * main.c
 *
 *  Created on: Jun 24, 2021
 *      Author: Mohamed Taha
 */

#define F_CPU 8000000
#include "keypad.h"
#include "lcd.h"
#include "timer.h"
#include "uart.h"

#define M1_READY 0x11
#define M2_READY 0x10

/* Commands to be sent to the control MC */
#define COMMAND_1 0x40
#define COMMAND_2 0x41
#define COMMAND_3 0x42
#define COMMAND_4 0x43
#define COMMAND_5 0x44

#define APP_IS_INITIALIZED 0xFA
#define APP_IS_NOT_INITIALIZED 0xF5

#define BUZZER_FINISH_FLAG 0xF8
#define MOTOR_FINISH_FLAG 0xF9



/* Global Variables */
uint8 g_password_array[5] = {0};
uint8 g_password_array_again[5] = {0};
uint8 g_password_array_user[5] = {0} ;

/* Function Prototypes */
void main_options(void);
void open_door_option(void);
void change_pass_option(void);
void write_first_time(void);
void check_first_time(void);

int main(void)
{
	/* Initialization */
	/*******************
	 *     Variables   *
	 *******************/

	/*******************
	 *    	LCD        *
	 *******************/
	LCD_init(); /* initialize LCD */

	/*******************
	 *    	 UART      *
	 *******************/
	/* Create configuration structure for UART driver */
	UART_ConfigType UART_Config = {EIGHT_Bit,Disabled,ONE_Bit,9600};
	UART_init(&UART_Config);
	UART_sendByte(M1_READY); // MC1 is ready
	while(UART_recieveByte() != M2_READY){} // wait until MC2 is ready

	while(1)
	{
		/* Application */
		check_first_time();
	}

	return 0;
}

void main_options(void)
{
	uint8 keypad_data = 0; //variable used to store data from keypad
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_goToRowColumn(1, 0);
	LCD_displayString("- : Change Pass");
	keypad_data = KeyPad_getPressedKey();
	_delay_ms(500);
	if(keypad_data == '+')
	{
		open_door_option();
		_delay_ms(500);
		main_options();
	}
	else if(keypad_data == '-')
	{
		change_pass_option();
		_delay_ms(500);
		main_options();
	}
	else
	{
		LCD_clearScreen();
		LCD_displayString("Wrong Option !");
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Try Again");
		_delay_ms(2000);
		main_options();
	}

}

void open_door_option(void)
{
	static uint8 count = 0;
	uint8 array_not_equal_flag = 0;
	LCD_clearScreen();
	LCD_displayString("Enter Pass:");
	LCD_goToRowColumn(1, 0);
	for(int i = 0 ; i < 5 ; i++)
	{
		g_password_array_user[i]= KeyPad_getPressedKey();
		_delay_ms(500); /* Press time */
		LCD_displayCharacter('*');
		_delay_ms(100);
	}
	for(int i = 0 ; i < 5 ; i++)
	{
		if(g_password_array_user[i] != g_password_array[i])
		{
			array_not_equal_flag = 1;
		}
	}
	if(array_not_equal_flag == 1)
	{
		count++;
		if(count == 3)
		{
			LCD_clearScreen();
			LCD_displayString("     ERROR   ");
			UART_sendByte(0x43); /* Send Command for Turning ON Buzzer */
			while(UART_recieveByte() != BUZZER_FINISH_FLAG){} // wait until BUZZER flag is ready
			count = 0;
			/* Back to main Options */
			main_options();
		}
		LCD_clearScreen();
		LCD_displayString("Wrong Pass.");
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Try Again");
		array_not_equal_flag = 0;
		_delay_ms(1000);
		open_door_option();
	}
	else
	{
		LCD_clearScreen();
		LCD_displayString("Right Password !");
		UART_sendByte(COMMAND_5); /* Send Command for Turning ON Buzzer */
		while(UART_recieveByte() != MOTOR_FINISH_FLAG){} // wait until MC2 is ready
		count=0;
		/* Back to main Options */
		main_options();
	}

}
void change_pass_option(void)
{
	static uint8 count = 0;
	uint8 array_not_equal_flag = 0;
	LCD_clearScreen();
	LCD_displayString("Enter old Pass:");
	LCD_goToRowColumn(1, 0);
	for(int i = 0 ; i < 5 ; i++)
	{
		g_password_array_user[i]= KeyPad_getPressedKey();
		_delay_ms(500); /* Press time */
		LCD_displayCharacter('*');
		_delay_ms(100);
	}
	for(int i = 0 ; i < 5 ; i++)
	{
		if(g_password_array_user[i] != g_password_array[i])
		{
			array_not_equal_flag = 1;
		}
	}
	if(array_not_equal_flag == 1)
	{
		count++;
		if(count == 3)
		{
			LCD_clearScreen();
			LCD_displayString("     ERROR   ");
			UART_sendByte(0x43); /* Send Command for Turning ON Buzzer */
			while(UART_recieveByte() != BUZZER_FINISH_FLAG){} // wait until BUZZER flag is ready
			count = 0;
			/* Back to main Options */
			main_options();
		}
		LCD_clearScreen();
		LCD_displayString("Wrong Pass.");
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Try Again");
		array_not_equal_flag = 0;
		_delay_ms(1000);
		change_pass_option();
	}
	else
	{
		LCD_clearScreen();
		LCD_displayString("Right Password !");
		_delay_ms(1000);
		write_first_time();
		/* Back to main Options */
		main_options();
	}
}
void write_first_time(void)
{
	LCD_clearScreen();
	LCD_displayString("Enter Pass:");
	uint8 array_not_equal_flag = 0;
	LCD_goToRowColumn(1, 0);
	for(int i = 0 ; i < 5 ; i++)
	{
		g_password_array[i]= KeyPad_getPressedKey();
		_delay_ms(500); /* Press time */
		LCD_displayCharacter('*');
		_delay_ms(100);
	}
	LCD_clearScreen();
	LCD_displayString("Enter same Pass:");
	LCD_goToRowColumn(1, 0);
	for(int i = 0 ; i < 5 ; i++)
	{
		g_password_array_again[i]= KeyPad_getPressedKey();
		_delay_ms(500); /* Press time */
		LCD_displayCharacter('*');
		_delay_ms(100);
	}
	for(int i = 0 ; i < 5 ; i++)
	{
		if(g_password_array[i] != g_password_array_again[i])
		{
			array_not_equal_flag = 1;
		}
	}
	if(array_not_equal_flag == 1)
	{
		write_first_time();
		array_not_equal_flag = 0;
	}
	else
	{
		LCD_clearScreen();
		LCD_displayString("Password Saved !");
		/* Sending Password to the Control MC to be saved in the E2PROM */
		UART_sendByte(COMMAND_2);
		for(int i = 0 ; i < 5 ; i++)
		{
			UART_sendByte(g_password_array[i]);
			while(UART_recieveByte() != M2_READY){} // wait until MC2 is ready
		}
		_delay_ms(500);
	}

}

void check_first_time(void)
{
	LCD_clearScreen();
	LCD_displayString("Welcome");
	UART_sendByte(COMMAND_1);
	uint8 receivedByte = 0 ; /* variable used to store data received from UART */
	receivedByte = UART_recieveByte();
	if(receivedByte == APP_IS_INITIALIZED) /* That means the App is initialized and the E2PROM has the password saved */
	{
		/* Send command to save the password from the E2PROM*/
		UART_sendByte(COMMAND_3);
		for(int i = 0 ; i < 5 ; i++)
		{
			receivedByte = UART_recieveByte();
			g_password_array[i] = receivedByte;
			UART_sendByte(M1_READY);
		}
		main_options();
	}
	else if(receivedByte == APP_IS_NOT_INITIALIZED) /* That means the App is not initialized and the user needs to save a password */
	{
		_delay_ms(1000);
		write_first_time();

	}

}
