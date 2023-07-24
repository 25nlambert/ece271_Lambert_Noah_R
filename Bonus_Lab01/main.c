#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

volatile uint32_t TimeDelay;

void SysTick_Initialize(uint32_t ticks) {
	
	//Disable SysTick
	SysTick->CTRL = 0;
	
	//Reload Register
	SysTick->LOAD = ticks - 1;
	
	//Set Interrupt Priority
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	//Reset counter value
	SysTick->VAL = 0;
	
	//Select Proccessor Clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	//Enables SysTick Interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	//Enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
}


void Delay(uint32_t nTime){
	
	TimeDelay = nTime;
	while(TimeDelay != 0);
	
}

void SysTick_Handler(void){
	
	if(TimeDelay > 0)
		TimeDelay--;
	
}

void configure_LED_pin(){
  // Enable the clock to GPIO Port A and C
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	//LED
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~GPIO_MODER_MODE5;  
	GPIOA->MODER |=   GPIO_MODER_MODE5_0;      // Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5; //Low Speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~GPIO_OTYPER_ODR_5;      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~GPIO_PUPDR_PUPDR5;  // No pull-up, no pull-down
	
	//BUTTON
	//Sets the GPIO mode to input (00), no value line required after
	GPIOC->MODER &= ~GPIO_MODER_MODE13; 

	//Sets GPIOC to no pull-up, no pull-down
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13; 
	
	//Buzzer
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~GPIO_MODER_MODE15;  
	GPIOA->MODER |=  GPIO_MODER_MODE15_0;      // Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED15; //Low Speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~GPIO_OTYPER_ODR_15;      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~GPIO_PUPDR_PUPDR15;  // No pull-up, no pull-down
}

int main(void){

	SysTick_Initialize(792);
	System_Clock_Init();
	configure_LED_pin();
	
	int i = 0;
	int j = 0;
	
	//Time in ms each tone needs to play for
	int time[26] = {250, 250, 500, 250, 250, 500, 250, 250, 250, 250, 1000, 250, 250, 250, 250, 250, 250, 250, 125, 125, 250, 250, 250, 250, 500, 250};
	
	//Frequency in Hz each tone needs to play at
	int frequency[26] = {329, 329, 329, 329, 329, 329, 329, 392, 261, 293, 329, 349, 349, 349, 349, 349, 329, 329, 329, 329, 329, 261, 261, 329, 261, 392};
	
	//How many notes there are in the song	
	int length = 26;
	
	int clock_freq = 2467917; 							 //How long the smallest delay is Delay(1)
	int freq = 440; 												 //440Hz test frequency
	int calculation = 100000000000/(2*freq); //Calculate 1/2 of a period
	int freq_delay = calculation/clock_freq; //Calculate the delay in relation to how long the system delay is.
		
	int tone_time_base = 246;												//250ms test tone length
	int tone_length = (time[1]*109)/tone_time_base; //Calculate the amount of times the tone needs to play for from ms

	while(1){
		
		//Play the 26 notes of the song Jingle Bells
		for(j = 0;j < 26; j++){
			
			calculation = 100000000000/(2*frequency[j]); //Calculate 1/2 of a period
			freq_delay = calculation/clock_freq;				 //Calculate the delay in relation to how long the system delay is.
		
			tone_length = (time[j]*109)/tone_time_base;  //Calculate the amount of times to play from the time array
			
			for(i = 0; i < tone_length; i++){           //Plays tone for proper length of time
			
					//Oscillate the buzzer at frequency calculated above to play proper note.
					GPIOA->ODR |= GPIO_ODR_OD15;
					Delay(freq_delay);
					GPIOA->ODR &= ~GPIO_ODR_OD15;
					Delay(freq_delay);
			
			}
			
			//Delay to add definition between the tones
			Delay(1000);
			
		}
		
		//Delay between the end and start of next play through to differentiate
		Delay(10000);
		
	}
	
}
