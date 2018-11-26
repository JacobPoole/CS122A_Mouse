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

 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h"
enum MOVEMOUSE {INIT,waitStart,check,left,right,turnAround,forward,turnWait,forwardWait} moveMouse;
//enum TRACKDATA {INITTrack, setData, send, wait } trackData;
//char where = "";
//int mat[16][16] = 0;


	
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	//ADEN: setting this bit enables analog-to-digital conversion.
	//ADSC: setting this bit starts the first conversion.
	//ADATE: setting this bit enables auto-triggering. Since we are
	//		 in Free Running Mode, a new conversion will trigger
	//		 whenever the previous conversion completes
}

int adc_read(int ch)
{
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
	
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}

void MOVE_Init(){
	moveMouse = INIT;
}
/*
void TRACK_Init(){
	trackData = INITTrack;
}*/

void MOVE_Tick(){
	//Actions
	switch(moveMouse){
		case INIT:
			
			srand(time(0));
			//char where = "";
			PORTC = 0x11;
			break;
		case waitStart:
			/*while(adcF < 1024){
				adcF = ADC;
			}
			while(adcF > 1024){
				adcF = ADC;
			}*/
			break;
		case check:  ;
			int adcL = adc_read(0);		// read adc value at PA0
			int adcR = adc_read(1);		// read adc value at PA1
			int adcF = adc_read(2);		// read adc value at PA2
			/*adcL = ADC; //get left adc value
			adcR = ADC; //get right ac value
			adcF = ADC; //get forward adc value
			
			
			if(adcL < 1024 && adcR < 1024 && adcF < 1024){  // all walls empty
				rand % 3; // 3 options
			}
			else if(adcL < 1024 && adcR < 1024){  // left and right wall empty
				rand % 2; // 2 options
			}
			else if(adcL < 1024 && adcF < 1024){  // left and front wall empty
				rand % 2; // 2 options
			}
			else if(adcR < 1024 && adcF < 1024){  // right and front wall empty
				rand % 2; // 2 options
			}
			else if(adcL < 1024){  // left wall empty
			
			}
			else if(adcR < 1024){  // right wall empty
			
			}
			else if(adcF < 1024){  // front wall empty
				where = "forward";
			}
			else{  // turn around
			
			}*/
			
			
			break;
		case left:  // turn left
			//where = "left";
			PORTC = 0x53;
			break;
		case right: // turn right
			//where = "right;"
			PORTC = 0x35;
			break;
		case turnAround: // turn around
			//where = "back";
			break;
		case forward: // move forward
			PORTC = 0x33;
			vTaskDelay(200);
			break;
		case turnWait:
			PORTC = 0x00;
			vTaskDelay(200);
			break;
		case forwardWait:
			PORTC = 0x00;	
			vTaskDelay(200);
			break;
		//trackData = setData;
	}
	//Transitions
	switch(moveMouse){
		case INIT:
			//moveMouse = waitStart;
			moveMouse = forward;
			break;
		case waitStart:
			//moveMouse = check;
			moveMouse = forward;
			break;
		case check: ;
			int r = rand()%3;
			if(r == 0){
				moveMouse = left;
			}
			else if(r == 1){
				moveMouse = right;
			}
			else{
				moveMouse = forward;
			}
			break;
		case left:
			moveMouse = turnWait;
			//moveMouse = right;
			break;
		case right:
			moveMouse = turnWait;
			break;
		case turnAround:
			moveMouse = turnWait;
			break;
		case turnWait:
			moveMouse = forward;
			break;
		case forward:
			moveMouse = forwardWait;
			//moveMouse = left;
			break;
		case forwardWait:
			moveMouse = check;
			break;
		
	}
}

/*
void TRACK_Tick(){
	//Actions
	switch(trackData){
		case INITTrack:
			break;
		case setData:  // set data change in matrix
			break;
		case send:  // USART send new change
			break;
		case wait:  // wait for new data change
			break;
	}
	switch(trackData){
		case INITTrack:
			trackData = setData;
			break;
		case setData:
			trackData = send;
			break;
		case send:
			trackData = wait;
			break;
		case wait:
			trackData = wait;
			break;
	}	
}
*/
void MoveSecTask()
{
	MOVE_Init();
   for(;;) 
   { 	
	MOVE_Tick();
	vTaskDelay(200); 
   } 
}
/*
void TrackSecTask(){
	TRACK_Init();
	for(;;){
		TRACK_Tick();
		vTaskDelay(100);	
	}
}*/

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(MoveSecTask, (signed portCHAR *)"MoveSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	//xTaskCreate(TrackSecTask, (signed portCHAR *)"TrackSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRC = 0xFF; 
   DDRD = 0xFF;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}