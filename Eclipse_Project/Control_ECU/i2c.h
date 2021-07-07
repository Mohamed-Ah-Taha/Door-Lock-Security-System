/*
 * i2c.h
 *
 *  Created on: Jun 23, 2021
 *      Author: Mohamed Taha
 */

#ifndef I2C_H_
#define I2C_H_

/* Include Common header files */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*******************************************************************************
 *         Preprocessor Macros (Definitions and static Configurations)         *
 *******************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave
/*******************************************************************************
 *       					  User-defined Types      						   *
 *******************************************************************************/
/* RATE1 means 100kbit/s mode . RATE2 means 400kbits/s mode */
typedef enum
{
	RATE1,RATE2
}TWI_BaudRate;

typedef uint8 TWI_Slave_Address;

typedef struct
{
	TWI_BaudRate Baud;
	TWI_Slave_Address Address;
}TWI_ConfigType;

/*******************************************************************************
 *       				  Extern for Public Global Variables   		 		   *
 *******************************************************************************/
extern volatile uint16 g_UART_Result;

/*******************************************************************************
 *       					  Functions Prototypes 	   				 		   *
 *******************************************************************************/
/*
 * Description : Function to initialize the TWI driver
 * 	1. Set the required Baud Rate.
 * 	2. Set the required Address if i am a slave.
 */
void TWI_init(const TWI_ConfigType * Config_Ptr);

void TWI_start(void);

void TWI_stop(void);

void TWI_write(uint8 data);

uint8 TWI_readWithACK(void); //read with send Ack

uint8 TWI_readWithNACK(void); //read without send Ack

uint8 TWI_getStatus(void);

#endif /* I2C_H_ */
