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
 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 
enum LEDState {INIT,on,off} led_state;
	
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	//ADEN: setting this bit enables analog-to-digital conversion.
	//ADSC: setting this bit starts the first conversion.
	//ADATE: setting this bit enables auto-triggering. Since we are
	//		 in Free Running Mode, a new conversion will trigger
	//		 whenever the previous conversion completes
}

void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
			PORTC = 0x00;
			PORTD = 0x00;
			break;
		case on:
			PORTC = (PORTC & 0xEF) | 0x10;
			PORTD = 0xFF;
			break;
		case off:
			PORTC = (PORTC & 0xEF) | 0x00;
			PORTD = 0x00;
			break;
		
	}
	//Transitions
	switch(led_state){
		case INIT:
			led_state = on;
		break;
		case on:
			led_state = off;
		break;
		case off:
			led_state = on;
		break;
		
	}
}

void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(100); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRC = 0xF0; PORTC=0x0F;	
   DDRD = 0xFF;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}