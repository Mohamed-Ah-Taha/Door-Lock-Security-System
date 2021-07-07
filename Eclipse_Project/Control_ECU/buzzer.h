/*
 * buzzer.h
 *
 *  Created on: Jun 24, 2021
 *      Author: Mohamed Taha
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/* Include Common header files */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*******************************************************************************
 *         Preprocessor Macros (Definitions and static Configurations)         *
 *******************************************************************************/

/* Motor Port Configurations */
#define BUZZER_DIR DDRA
#define BUZZER_PORT PORTA
#define BUZZER_PIN PA1

/*******************************************************************************
 *       					  User-defined Types      						   *
 *******************************************************************************/

/*******************************************************************************
 *       				  Extern for Public Global Variables   		 		   *
 *******************************************************************************/


/*******************************************************************************
 *       					  Functions Prototypes 	   				 		   *
 *******************************************************************************/
void Buzzer_ON(void);

void Buzzer_OFF(void);

#endif /* BUZZER_H_ */
