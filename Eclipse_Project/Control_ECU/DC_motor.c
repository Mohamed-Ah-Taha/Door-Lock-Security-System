/*
 * DC_motor.c
 *
 *  Created on: Jun 2, 2021
 *      Author: Mohamed Taha
 */


/* Include Module.h file */
#include "DC_motor.h"

/*******************************************************************************
 *       				  		Global Variables   			 		  		   *
 *******************************************************************************/

/* Public (Shared) Global Variables */

/* Private (Static) Global Variables */


/*******************************************************************************
 *       				Functions Prototypes(Private)  			 		  	   *
 *******************************************************************************/



/*******************************************************************************
 *       				Functions Definitions		  			 		  	   *
 *******************************************************************************/

/* Public (Shared) Functions */
void DCMotor_init(void)
{
	/* Setting the MC pins as output to the input of the H-bridge */
	SET_BIT(H_BRIDGE_IN_DIR,H_BRIDGE_IN1);
	SET_BIT(H_BRIDGE_IN_DIR,H_BRIDGE_IN2);

	/* Motor is stop at the beginning */
	CLEAR_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN1);
	CLEAR_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN2);
}

void DCMotor_stop(void)
{
	CLEAR_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN1);
	CLEAR_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN2);
}
void DCMotor_clockwise(void)
{
	CLEAR_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN1);
	SET_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN2);
}

void DCMotor_anti_clockwise(void)
{
	SET_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN1);
	CLEAR_BIT(H_BRIDGE_IN_PORT,H_BRIDGE_IN2);
}

/* Private (Static) Functions */
