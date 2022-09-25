/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
void task1( void * pvParameters );
void task2( void * pvParameters );
void task3( void * pvParameters );
void task4( void * pvParameters );
void task5( void * pvParameters );
void task6( void * pvParameters );

TaskHandle_t task1Handler = NULL;
TaskHandle_t task2Handler = NULL;
TaskHandle_t task3Handler = NULL;
TaskHandle_t task4Handler = NULL;
TaskHandle_t task5Handler = NULL;
TaskHandle_t task6Handler = NULL;

char rising_task1_button [27] = "rising edge task1 string\n";
char rising_task2_button [27] = "rising edge task2 string\n";
char falling_task1_button [28]= "falling edge task1 string\n";
char falling_task2_button [28]= "falling edge task2 string\n";
char period_string [18]= "periodic string\n";


// global flags to establish communication between tasks
int cond1 = 0;  //task 1 falling
int cond2 = 0;  //task 1 rising
int cond3 = 0;  //task 2 falling
int cond4 = 0;  //task 2 rising

pinState_t prev_statebutton1=PIN_IS_LOW;
pinState_t curr_statebutton1=PIN_IS_LOW;
pinState_t prev_statebutton2=PIN_IS_LOW;
pinState_t curr_statebutton2=PIN_IS_LOW;


void task1( void * pvParameters ){     //button 1 monitor

		int i=0;
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 50;

     // Initialise the xLastWakeTime variable with the current time.
     xLastWakeTime = xTaskGetTickCount();


		vTaskSetApplicationTaskTag(NULL, (void *) 1 );

    for( ;; )
    {
			
		curr_statebutton1=GPIO_read(PORT_0, PIN0);
		
		if(curr_statebutton1!=prev_statebutton1){
			if ( curr_statebutton1 == PIN_IS_LOW ){
				cond1=0;
			}
			else {
				cond1=1;
			}
		}
		else{
			cond1=3;
		}
		prev_statebutton1=curr_statebutton1;
		
		//vTaskDelay(50);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}

void task2( void * pvParameters ){     //button 2 monitor

		int i=0;
			TickType_t xLastWakeTime;
		const TickType_t xFrequency = 50;
	
	     xLastWakeTime = xTaskGetTickCount();

		vTaskSetApplicationTaskTag(NULL, (void *) 2 );

    for( ;; )
    {
			
		curr_statebutton2=GPIO_read(PORT_0, PIN1);
		
		if(curr_statebutton2!=prev_statebutton2){
			if ( curr_statebutton2 == PIN_IS_LOW ){
				cond2=0;
			}
			else {
				cond2=1;
			}
		}
		else{
			cond2=3;
		}
		prev_statebutton2=curr_statebutton2;
		//vTaskDelay(50);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );

    }
}


void task3( void * pvParameters ){     //Periodic

			TickType_t xLastWakeTime;
		const TickType_t xFrequency = 100;
     xLastWakeTime = xTaskGetTickCount();

		vTaskSetApplicationTaskTag(NULL, (void *) 3 );

    for( ;; )
    {
		cond3=1;
//		vTaskDelay(100);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );

    }
}


void task4( void * pvParameters ){     //UART transmitter task
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 20;
     xLastWakeTime = xTaskGetTickCount();

		vTaskSetApplicationTaskTag(NULL, (void *) 4 );


    for( ;; )
    {
		if (cond1 ==0 ){  //task1 button low
			
			//send "falling edge task1 string"
			vSerialPutString(NULL,falling_task1_button, 28);
			
		}
		if (cond1 ==1 ){	//task1 button high
			
			//send "rising edge task1 string"
			vSerialPutString(NULL,rising_task1_button, 27);
			
			cond1=3;				//reset the flag
		}
		
		
		
		if (cond2 ==0 ){	//task2 button low
			
			//send "falling edge task2 string"
			vSerialPutString(NULL,falling_task2_button, 28);

			
		}
		if (cond2 ==1 ){	//task2 button high
			
			//send "rising edge task2 string"
			vSerialPutString(NULL,rising_task2_button, 27);

			
			cond2=3;				//reset the flag
		}

		
		
		if (cond3 ==1 ){	//periodic task sent string
			
			//send "periodic string"
			vSerialPutString(NULL,period_string, 18);
			
			cond3=3;				//reset the flag
		}
//		vTaskDelay(20);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );

    }
}



void task5( void * pvParameters ){		//button 2 monitor

	
	
		int i=0;
			TickType_t xLastWakeTime;
		const TickType_t xFrequency = 10;
     xLastWakeTime = xTaskGetTickCount();

		vTaskSetApplicationTaskTag(NULL, (void *) 5 );
	
    for( ;; )
		{
			for( i=0; i<37000; i++){
				i=i;
			}
	//			vTaskDelay(10);
			
				vTaskDelayUntil( &xLastWakeTime, xFrequency );
			}
}

void task6( void * pvParameters ){		//button 2 monitor

	
	
		int i=0;
			TickType_t xLastWakeTime;
		const TickType_t xFrequency = 100;
     xLastWakeTime = xTaskGetTickCount();

		vTaskSetApplicationTaskTag(NULL, (void *) 6 );
	
    for( ;; )
		{
			for( i=0; i<87995; i++){
				i=i;
			}
			
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
}

void vApplicationIdleHook(){
	
//	GPIO_write(PORT_0, PIN4, PIN_IS_LOW);
//	GPIO_write(PORT_0, PIN4, PIN_IS_HIGH);
}

void vApplicationTickHook(){
	
//	GPIO_write(PORT_0, PIN4, PIN_IS_LOW);
//	GPIO_write(PORT_0, PIN4, PIN_IS_HIGH);

}

/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	
	xTaskCreatePeriodic(
                    task1,       /* Function that implements the task. */
                    "task1",          /* Text name for the task. */
                    200,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task1Handler, 
										50);      /* Used to pass out the created task's handle. */
								
	xTaskCreatePeriodic(
                    task2,       /* Function that implements the task. */
                    "task2",          /* Text name for the task. */
                    200,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task2Handler, 
										50);      /* Used to pass out the created task's handle. */
										
										
	xTaskCreatePeriodic(
                    task3,       /* Function that implements the task. */
                    "task3",          /* Text name for the task. */
                    200,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task3Handler,
										100);      /* Used to pass out the created task's handle. */
										
										
	xTaskCreatePeriodic(
                    task4,       /* Function that implements the task. */
                    "task4",          /* Text name for the task. */
                    200,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task4Handler,
										20);      /* Used to pass out the created task's handle. */
										
										
										
										xTaskCreatePeriodic(
                    task5,       /* Function that implements the task. */
                    "task5",          /* Text name for the task. */
                    200,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task5Handler,
										10);      /* Used to pass out the created task's handle. */
										
										
										
										xTaskCreatePeriodic(
                    task6,       /* Function that implements the task. */
                    "task6",          /* Text name for the task. */
                    200,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task6Handler,
										100);      /* Used to pass out the created task's handle. */
										
										


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE,100);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


