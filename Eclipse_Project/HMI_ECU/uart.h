/*
 * uart.h
 *
 *  Created on: Jun 22, 2021
 *      Author: Mohamed Taha
 */

#ifndef UART_H_
#define UART_H_

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
	FIVE_Bit,SIX_Bit,SEVEN_Bit,EIGHT_Bit,NINE_Bit=7
}UART_Data;

typedef enum
{
	Disabled,Enabled_Even=2,Enabled_Odd
}UART_Parity;

typedef enum
{
	ONE_Bit,TWO_Bit
}UART_Stop_Bit;


typedef uint32 UART_BAUDRATE;

typedef struct
{
	UART_Data Data;
	UART_Parity Parity;
	UART_Stop_Bit Stop;
	UART_BAUDRATE Baud;
}UART_ConfigType;

/*******************************************************************************
 *       				  Extern for Public Global Variables   		 		   *
 *******************************************************************************/
extern volatile uint16 g_UART_Result;

/*******************************************************************************
 *       					  Functions Prototypes 	   				 		   *
 *******************************************************************************/
/*
 * Description : Function to initialize the UART driver
 * 	1. Set the number of bits of Data in the UART frame.
 * 	2. Set the number of Parity bits in the UART frame.
 * 	3. Set the number of Stop bits in the UART frame.
 * 	4. Set the Baud Rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #
#endif /* UART_H_ */
