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
enum MOVEMOUSE {INIT,waitStart,check,left,right,turnAround,forward,turnWait,forwardWait} moveMouse;
enum EMITLIGHT {INITEmit, on, off} emitLight;
enum TRACKDATA {INITTrack, send, wait } trackData;
int dir = 100;
int data[4][4] = {0};
int j, k = 0;
int sendValue = 0;
int flag1, flag2, flag3, flag4, flag5, flag6, flag7, flag8, flag9, flag10, flag11, flag12, flag13, flag14, flag15, flag16 = 0;

volatile unsigned char ADC_Flag = 0;


	
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADATE);
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
	//ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
	
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	return (ADC);
}

ISR(ADC_vect){
	ADC_Flag = 1;
}

void MOVE_Init(){
	moveMouse = INIT;
}

void TRACK_Init(){
	trackData = INITTrack;
}

void EMIT_Init(){
	emitLight = INITEmit;
}

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
			data[0][0] = 1;
			moveMouse = forward;
			break;
		case waitStart:
			//moveMouse = check;
			moveMouse = forward;
			break;
		case check: ;
			ADC_init();
			int adcL = adc_read(0);		// read adc value at PA0
			while(!ADC_Flag){} ADC_Flag = 0;
			int adcR = adc_read(1);		// read adc value at PA1
			while(!ADC_Flag){} ADC_Flag = 0;
			int adcF = adc_read(2);		// read adc value at PA2
			while(!ADC_Flag){} ADC_Flag = 0;
			PORTD = 0xFF;
			/*adcL = ADC; //get left adc value
			adcR = ADC; //get right ac value
			adcF = ADC; //get forward adc value*/
			int r = 0;
			
			/*if(adcL < 250 && adcR < 250 && adcF < 250){  // all walls empty
				r = rand() % 3; // 3 options
				if(r == 0){
					moveMouse = left;
				}
				else if(r == 1){
					moveMouse = right;
				}
				else{
					moveMouse = forward;
				}
			}
			else if(adcL < 250 && adcR < 250){  // left and right wall empty
				r = rand() % 2; // 2 options
				if(r == 0){
					moveMouse = left;
				}
				else{
					moveMouse = right;
				}
			}
			else if(adcL < 250 && adcF < 250){  // left and front wall empty
				r = rand() % 2; // 2 options
				if(r == 0){
					moveMouse = left;
				}
				else{
					moveMouse = forward;
				}
			}
			else if(adcR < 250 && adcF < 250){  // right and front wall empty
				r = rand() % 2; // 2 options
				if(r == 0){
					moveMouse = right;
				}
				else{
					moveMouse = forward;
				}
			}
			else if(adcL < 250){  // left wall empty
				moveMouse = left;
			}
			else if(adcR < 250){  // right wall empty
				moveMouse = right;
			}
			else if(adcF < 250){  // front wall empty
				//where = "forward";
				moveMouse = forward;
			}
			else{  // turn around
			
			}*/
			
			if(adcL == 1008){  // left wall empty
				moveMouse = left;
			}
			
			
		
		
		
			/*int r = rand()%3;
			if(r == 0){
				moveMouse = left;
			}
			else if(r == 1){
				moveMouse = right;
			}
			else{
				moveMouse = forward;
			}*/
			break;
		case left:
			dir += 1;
			moveMouse = turnWait;
			//moveMouse = right;
			break;
		case right:
			dir -= 1;
			moveMouse = turnWait;
			break;
		case turnAround:
			dir += 2;
			moveMouse = turnWait;
			break;
		case turnWait:
			moveMouse = forward;
			break;
		case forward:
			if(dir%4 == 0){
				k += 1;
				data[j][k] = 1;
			}
			else if(dir%4 == 1){
				j += 1;
				data[j][k] = 1;
			}
			else if(dir%4 == 2){
				k -= 1;
				data[j][k] = 1;
			}
			else if(dir%4 == 3){
				j -= 1;
				data[j][k] = 1;
			}
			
			moveMouse = forwardWait;
			//moveMouse = left;
			break;
		case forwardWait:
			moveMouse = check;
			break;
		
	}
}


void TRACK_Tick(){
	//Actions
	switch(trackData){
		case INITTrack:
			break;
		case send:  // USART send new change
			//check each value in matrix and add to the send value if it hasn't already been added to.
			if(data[0][0] == 1 && flag1 == 0){
				sendValue += 1;
				flag1 = 1;
			}
			else if(data[0][1] == 1 && flag2 == 0){
				sendValue += 2;
				flag2 = 1;
			}
			else if(data[0][2] == 1 && flag3 == 0){
				sendValue += 3;
				flag3 = 1;
			}
			else if(data[0][3] == 1 && flag4 == 0){
				sendValue += 4;
				flag4 = 1;
			}
			else if(data[1][0] == 1 && flag5 == 0){
				sendValue += 5;
				flag5 = 1;
			}
			else if(data[1][1] == 1 && flag6 == 0){
				sendValue += 6;
				flag6 = 1;
			}
			else if(data[1][2] == 1 && flag7 == 0){
				sendValue += 7;
				flag7 = 1;
			}
			else if(data[1][3] == 1 && flag8 == 0){
				sendValue += 8;
				flag8 = 1;
			}
			else if(data[2][0] == 1 && flag9 == 0){
				sendValue += 9;
				flag9 = 1;
			}
			else if(data[2][1] == 1 && flag10 == 0){
				sendValue += 10;
				flag10 = 1;
			}
			else if(data[2][2] == 1 && flag11 == 0){
				sendValue += 11;
				flag11 = 1;
			}
			else if(data[2][3] == 1 && flag12 == 0){
				sendValue += 12;
				flag12 = 1;
			}
			else if(data[3][0] == 1 && flag13 == 0){
				sendValue += 13;
				flag13 = 1;
			}
			else if(data[3][1] == 1 && flag14 == 0){
				sendValue += 14;
				flag14 = 1;
			}
			else if(data[3][2] == 1 && flag15 == 0){
				sendValue += 15;
				flag15 = 1;
			}
			else if(data[3][3] == 1 && flag16 == 0){
				sendValue += 16;
				flag16 = 1;
			}
			
			//actually send "sendValue" here
			if( USART_IsSendReady(0))
			{
				USART_Send(sendValue);
				USART_Flush(0);
			}
			
			
			break;
		case wait:  // wait for new data change
			break;
	}
	switch(trackData){
		case INITTrack:
			trackData = send;
			break;
		case send:
			trackData = wait;
			break;
		case wait:
			trackData = send;
			break;
	}	
}


void EMIT_Tick(){
	switch(emitLight){
		case INITEmit:
			emitLight = on;
			break;
		case on:
			PORTA = (PORTA & 0x8F) | 0x70;
			emitLight = on;
			break;
		case off:
			PORTA  = (PORTA & 0x8F) | 0x00;
			emitLight = on;
			break;
	}
	
}
void MoveSecTask()
{
	MOVE_Init();	
   for(;;) 
   { 	
	MOVE_Tick();
	vTaskDelay(200); 
   } 
}

void EmitSecTask(){
	EMIT_Init();
	for(;;){
		EMIT_Tick();
		vTaskDelay(100);
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
	xTaskCreate(MoveSecTask, (signed portCHAR *)"MoveSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(TrackSecTask, (signed portCHAR *)"TrackSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(EmitSecTask, (signed portCHAR *)"EmitSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
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