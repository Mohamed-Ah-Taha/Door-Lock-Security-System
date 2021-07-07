/*
 * uart.c
 *
 *  Created on: Jun 22, 2021
 *      Author: Mohamed Taha
 */


/* Include Module.h file */
#include "uart.h"

/*******************************************************************************
 *       				  		Global Variables   			 		  		   *
 *******************************************************************************/

/* Public (Shared) Global Variables */

volatile uint16 g_UART_Result = 0;
/* Private (Static) Global Variables */

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
/* UART RX Complete ISR */
ISR(USART_RXC_vect)
{
	g_UART_Result = UDR;
}

/*******************************************************************************
 *       				Functions Prototypes(Private)  			 		  	   *
 *******************************************************************************/




/*******************************************************************************
 *       				Functions Definitions		  			 		  	   *
 *******************************************************************************/

/* Public (Shared) Functions */
/*
 * Description : Function to initialize the UART driver
 * 	1. Set the number of bits of Data in the UART frame.
 * 	2. Set the number of Parity bits in the UART frame.
 * 	3. Set the number of Stop bits in the UART frame.
 * 	4. Set the Baud Rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint32 BAUD_PRESCALE = (((F_CPU / ((Config_Ptr->Baud) * 8))) - 1);
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 1 Enable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART TX Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1<<RXCIE) | (1<<RXEN) | (1<<TXEN);
	if((Config_Ptr->Data)==NINE_Bit)
	{
		SET_BIT(UCSRB,UCSZ2);
	}

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit , 10 Enable Even Parity , 11 Enable Odd Parity
	 * USBS    = 0 One stop bit , 1 Two stop bits
	 * UCSZ1:0 = 00(5-bit mode) , 01 (6-bit mode) , 10 (7-bit mode) , 11 (8-bit mode)
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC = (1<<URSEL) | (UCSRC&0xC1) | (((Config_Ptr->Data)<<1)&0x06) | (((Config_Ptr->Parity)<<4)&0x30) | (((Config_Ptr->Stop)<<3)&0x08);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ((uint8)BAUD_PRESCALE)>>8;
	UBRRL =  (uint8)BAUD_PRESCALE;

}

void UART_sendByte(const uint8 data)
{
	/* UDRE flag is set when the TX buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
}

uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
    return UDR;
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = g_UART_Result;
	while(Str[i] != '#')
	{
		i++;
		Str[i] = g_UART_Result;
	}
	Str[i] = '\0';
}
/* Private (Static) Functions */

