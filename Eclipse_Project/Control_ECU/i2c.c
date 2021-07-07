/*
 * i2c.c
 *
 *  Created on: Jun 23, 2021
 *      Author: Mohamed Taha
 */


/* Include Module.h file */
#include "i2c.h"

/*******************************************************************************
 *       				  		Global Variables   			 		  		   *
 *******************************************************************************/

/* Public (Shared) Global Variables */

/* Private (Static) Global Variables */

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/*******************************************************************************
 *       				Functions Prototypes(Private)  			 		  	   *
 *******************************************************************************/




/*******************************************************************************
 *       				Functions Definitions		  			 		  	   *
 *******************************************************************************/

/* Public (Shared) Functions */
/*
 * Description : Function to initialize the TWI driver
 * 	1. Set the required Baud Rate.
 * 	2. Set the required Address if i am a slave.
 */
void TWI_init(const TWI_ConfigType * Config_Ptr)
{
	if((Config_Ptr->Baud)==RATE2) /* 400 kbps mode*/
	{
	    /* Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz */
	    TWBR = 0x02;
		TWSR = 0x00;
	}
	else if((Config_Ptr->Baud)==RATE1) /* 100 kbps mode*/
	{
	    /* Bit Rate: 100.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz */
	    TWBR = 0x20;
		TWSR = 0x00;
	}
    /* Two Wire Bus address my address if any master device want to call me (used in case this MC is a slave device)
       General Call Recognition: Off */
    TWAR = (Config_Ptr->Address);

    TWCR = (1<<TWEN); /* enable TWI */
}

void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_write(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_readWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}
/* Private (Static) Functions */

