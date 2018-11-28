#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 
#include <time.h>
#include "usart.h"

 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h"
enum TRACKDATA {INITTrack, set, wait } trackData;
static unsigned char receive_data;


void TRACK_Init(){
	trackData = INITTrack;
}

void TRACK_Tick(){
	//Actions
	switch(trackData){
		case INITTrack:
			break;
		case set:  // USART receive new change
			if (USART_HasReceived(0))
			{
				receive_data += USART_Receive(0);
			}
			break;
		case wait:  // wait for new data change
			break;
	}
	switch(trackData){
		case INITTrack:
			trackData = set;
			break;
		case set:
			trackData = wait;
			break;
		case wait:
			trackData = set;
			break;
	}	
}


void TrackSecTask(){
	TRACK_Init();
	for(;;){
		TRACK_Tick();
		vTaskDelay(100);	
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(TrackSecTask, (signed portCHAR *)"TrackSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0xF0; PORTA=0xFF;
   DDRC = 0xFF; 
   DDRD = 0xFF;
   initUSART();
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}