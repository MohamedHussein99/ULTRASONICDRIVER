/*
 * ultrasonice.c
 *
 *  Created on: Oct 13, 2021
 *      Author: Mohamed
 */
#include "ultrasonic.h"
#include "gpio.h"
#include "common_macros.h"
#include "icu.h"
#include <avr/io.h>
#include <util/delay.h>
/*global variable*/
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 distance=0;

/*
 * Description :
 * Initialize the ICU driver as required.
 *  Setup the ICU call back function.
Setup the direction for the trigger pin as output pin through the GPIO driver.
 */
void Ultrasonic_init(void)
{
	Icu_ConfigType Icu_ConfigType ={F_CPU_8,RISING};  /*Configuration the ICU*/
	Icu_init(&Icu_ConfigType);            /* Initialization ICU */
	 GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT);/*Trigger pin*/
	Icu_setCallBack(Ultrasonic_edgeProcessing);   /*call back function*/
	TRIGOFF; /* init trig pin off*/

}
/*
 * Description :
*Send the Trigger pulse to the ultrasonic.
 */
void Ultrasonic_Trigger(void)
		{
	TRIGON;
	_delay_us(10);
	TRIGOFF;



		}
/*
 * Description :
*Send the trigger pulse by using Ultrasonic_Trigger function.
 *Start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();            /*Call trigger function*/

	distance=g_timeHigh/(57.8);

	return distance;
}

/*Description
* This is the call back function called by the ICU driver.
* This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
*/

void Ultrasonic_edgeProcessing(void)
{


		if(g_edgeCount == 0)
		{
			/*
			 * Clear the timer counter register to start measurements from the
			 * first detected rising edge
			 */

			 Icu_clearTimerValue();

			/* Detect falling edge */
				Icu_setEdgeDetectionType(FALLING);
				/* to detect Rising for next edge*/
				g_edgeCount=1;


		}
		else if(g_edgeCount == 1)
		{
			/* Store the High time value */

			g_timeHigh = Icu_getInputCaptureValue();
            /* Detect rising edge */
			Icu_setEdgeDetectionType(RISING);
			/* to detect Falling for next edge*/

			g_edgeCount=0;
		}
}

