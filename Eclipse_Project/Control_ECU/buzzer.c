/*
 * buzzer.c
 *
 *  Created on: Jun 24, 2021
 *      Author: Mohamed Taha
 */


/* Include Module.h file */
#include "buzzer.h"

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
void Buzzer_ON(void)
{
	SET_BIT(BUZZER_DIR,BUZZER_PIN);
	SET_BIT(BUZZER_PORT,BUZZER_PIN);
}

void Buzzer_OFF(void)
{
	CLEAR_BIT(BUZZER_PORT,BUZZER_PIN);
}
