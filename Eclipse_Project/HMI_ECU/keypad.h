/*
 * keypad.h
 *
 *  Created on: May 30, 2021
 *      Author: Mohamed Taha
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_


/* Include Common header files */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*******************************************************************************
 *         Preprocessor Macros (Definitions and static Configurations)         *
 *******************************************************************************/

/* Keypad configurations for number of rows and columns */
#define N_col 4
#define N_row 4

/* Keypad Port Configurations */
#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_IN  PINA
#define KEYPAD_PORT_DIR DDRA


/*******************************************************************************
 *       					  User-defined Types      						   *
 *******************************************************************************/


/*******************************************************************************
 *       				  Extern for Public Global Variables   		 		   *
 *******************************************************************************/


/*******************************************************************************
 *       					  Functions Prototypes 	   				 		   *
 *******************************************************************************/

/* Function responsible for getting the pressed keypad key */
uint8 KeyPad_getPressedKey(void);

#endif /* KEYPAD_H_ */
