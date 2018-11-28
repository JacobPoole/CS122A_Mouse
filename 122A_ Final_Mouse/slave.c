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
enum TRACKDATA {INITTrack, receive, wait } trackData;
enum SETDATA {INITSet, set } setData;
static unsigned char receive_data;
int flag1, flag2, flag3, flag4, flag5, flag6, flag7, flag8, flag9, flag10, flag11, flag12, flag13, flag14, flag15 = 0;


void TRACK_Init(){
	trackData = INITTrack;
}

void SET_Init(){
	setData = INITSet;
}

void TRACK_Tick(){
	//Actions
	switch(trackData){
		case INITTrack:
			break;
		case receive:  // USART receive new change
			if (USART_HasReceived(0))
			{
				receive_data += USART_Receive(0);
				PORTA = receive_data;
			}
			break;
		case wait:  // wait for new data change
			break;
	}
	switch(trackData){
		case INITTrack:
			trackData = receive;
			break;
		case receive:
			trackData = wait;
			break;
		case wait:
			trackData = receive;
			break;
	}	
}

void SET_Tick(){
	//Actions
	switch(setData){
		case INITSet:
			// set light [0][0] to on already
			break;
		case set: //use receive_data to set matrix. Start at 15 and widdle down to 0 using flags & if statements
			if(receive_data == 15 && flag15 == 0){
						//Set [3][3] on
				flag15 = 1;
			}
			else if(receive_data == 14 && flag14 == 0){
						//Set [3][2] on
				flag14 = 1;
			}
			else if(receive_data == 13 && flag13 == 0){
				//Set [3][1] on
				flag13 = 1;
			}
			else if(receive_data == 12 && flag12 == 0){
				//Set [3][0] on
				flag12 = 1;
			}
			else if(receive_data == 11 && flag11 == 0){
				//Set [2][3] on
				flag11 = 1;
			}
			else if(receive_data == 10 && flag10 == 0){
				//Set [2][2] on
				flag10 = 1;
			}
			else if(receive_data == 9 && flag9 == 0){
				//Set [2][1] on
				flag9 = 1;
			}
			else if(receive_data == 8 && flag8 == 0){
				//Set [2][0] on
				flag8 = 1;
			}
			else if(receive_data == 7 && flag7 == 0){
				//Set [1][3] on
				flag7 = 1;
			}
			else if(receive_data == 6 && flag6 == 0){
				//Set [1][2] on
				flag6 = 1;
			}
			else if(receive_data == 5 && flag5 == 0){
				//Set [1][1] on
				flag5 = 1;
			}
			else if(receive_data == 4 && flag4 == 0){
				//Set [1][0] on
				flag4 = 1;
			}
			else if(receive_data == 3 && flag3 == 0){
				//Set [0][3] on
				flag3 = 1;
			}
			else if(receive_data == 2 && flag2 == 0){
				//Set [0][2] on
				flag2 = 1;
			}
			else if(receive_data == 1 && flag1 == 0){
				//Set [0][1] on
				flag1 = 1;
			}
			
			break;
	}
	switch(trackData){
		case INITSet:
			setData = set;
			break;
		case receive:
			setData = set;
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
	xTaskCreate(SetSecTask, (signed portCHAR *)"SetSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
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