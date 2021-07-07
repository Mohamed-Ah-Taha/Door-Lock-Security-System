/*
 * DC_motor.h
 *
 *  Created on: Jun 2, 2021
 *      Author: Mohamed Taha
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/* Include Common header files */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*******************************************************************************
 *         Preprocessor Macros (Definitions and static Configurations)         *
 *******************************************************************************/

/* Motor Port Configurations */
#define H_BRIDGE_IN_DIR DDRB
#define H_BRIDGE_IN_PORT PORTB

#define H_BRIDGE_IN1 PB6
#define H_BRIDGE_IN2 PB7


/*******************************************************************************
 *       					  User-defined Types      						   *
 *******************************************************************************/

/*******************************************************************************
 *       				  Extern for Public Global Variables   		 		   *
 *******************************************************************************/


/*******************************************************************************
 *       					  Functions Prototypes 	   				 		   *
 *******************************************************************************/
void DCMotor_init(void);

void DCMotor_stop(void);

void DCMotor_clockwise(void);

void DCMotor_anti_clockwise(void);

#endif /* DC_MOTOR_H_ */
