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

void configure_KEYPAD_pin() {
	
	//GPIO Port C already enabled  in configure_MOTOR_BUTTON_pin
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	//ROW
	GPIOC->MODER &= ~GPIO_MODER_MODE0;  
	GPIOC->MODER |=  GPIO_MODER_MODE0_0;      //  R1 PC0 Output(01)
	
	GPIOC->MODER &= ~GPIO_MODER_MODE1;  
	GPIOC->MODER |=  GPIO_MODER_MODE1_0;      //  R2 PC1
	
	GPIOC->MODER &= ~GPIO_MODER_MODE2;  
	GPIOC->MODER |=  GPIO_MODER_MODE2_0;      //  R3 PC2
	
	GPIOC->MODER &= ~GPIO_MODER_MODE3;  
	GPIOC->MODER |=  GPIO_MODER_MODE3_0;      //  R4 PC3
	
	//COLUMN
	GPIOC->MODER &= ~GPIO_MODER_MODE4;  			//  C1 PC4 Input(00)
	
	GPIOC->MODER &= ~GPIO_MODER_MODE10;				//  C2 PC10
	
	GPIOC->MODER &= ~GPIO_MODER_MODE11;	      //  C3 PC11
	
	GPIOC->MODER &= ~GPIO_MODER_MODE12;				//  C4 PC12
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	//ROW
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED0;  // R1 PC0 Low speed
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1;  // R2 PC1
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED2;  // R3 PC2
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED3;  // R4 PC3
	
	//COLUMN
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED4;  // C1 PC4
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED10; // C2 PC10
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED11; // C3 PC11
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED12; // C4 PC12
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOC->OTYPER |= GPIO_OTYPER_OT0;      		// R1 PC0 Open Drain
	
	GPIOC->OTYPER |= GPIO_OTYPER_OT1;     	 	// R2 PC1
	
	GPIOC->OTYPER |= GPIO_OTYPER_OT2;     	 	// R3 PC2
	
	GPIOC->OTYPER |= GPIO_OTYPER_OT3;     	 	// R4 PC3
	
/*	GPIOC->OTYPER &= ~GPIO_OTYPER_OT4;      	// C1 PC4
	
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT10;     	// C2 PC10
	
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT11;     	// C3 PC11
	
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT12;     	// C4 PC12*/
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	//ROW
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD0;  			// R1 PC0 No pull-up, no pull-down
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD1;  			// R2 PC1
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD2;  			// R3 PC2
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD3;  			// R4 PC3
	
	//COLUMN
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD4;  			// C1 PC4 No pull-up, no pull-down
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD10;  			// C2 PC10
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD11;				// C3 PC11
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD12;  			// C4 PC12
	
}

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
}



int main(void){

	SysTick_Initialize(7992);
	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();
	configure_KEYPAD_pin();
	
	//Keypad Variables
	char col, pressed_col, row, pressed_row = 0; //Keeps track of iteration and which button is pressed
	int cols[] = {4, 10, 11, 12};								 //Column GPIO Pins
	int rows[] = {0, 1, 2, 3};									 //Row GPIO Pins
	int input_mask = 0x1C10;										 //For masking everything but input pins
	int output_to_one = 0xF;										 //For setting rows bit to 1
	char COL_ROW_DEBUG[64] = "";								 //Holds COL and ROW data for OLED
	int delay_button = 500;
	char key_map [4][4] = {											 //Maps keys to each col and row
		{1, 2, 3, 'A'},
		{4, 5, 6, 'B'},
		{7, 8, 9, 'C'},
		{'*', '0', '#', 'D'},
	};
	
	char current_key = 0x0;										//Holds the current key in ASCII form
	int i = 0;
	
  // Dead loop & program hangs here
	while(1){
		
		GPIOC->ODR &= ~GPIO_ODR_OD0;
		GPIOC->ODR &= ~GPIO_ODR_OD1;
		GPIOC->ODR &= ~GPIO_ODR_OD2;
		GPIOC->ODR &= ~GPIO_ODR_OD3; //Sets 0000 to all rows for column reading
		
		//Waits for a keypad press for scanning
		while(((GPIOC->IDR & input_mask)) == input_mask);
		
		//Column Scan
		for(col = 0; col < 4; col++){													//Finds the column pressed
			if((GPIOC->IDR & (1<<cols[col])) == 0){
				pressed_col = col;
			}
		}
		
		//Debouncer
		Delay(20);
		
		//Sets all output bits to 1 for row scan
		GPIOC->ODR |= output_to_one; 														
		
		//Row Scan
		for(row = 0; row < 4; row++) {
			GPIOC->ODR &= ~(1<<rows[row]); 											//Itterates setting the current bit to 0
			if((GPIOC->IDR & (1<<cols[col])) == 0) {
				pressed_row = row;
			}
			GPIOC->ODR |= (1<<rows[row]); 											//Resets current bit to 1
		}
		
		current_key = key_map[pressed_row][pressed_col];			//Retrieves what the key represents from the map
		
		if((current_key != 0) & (current_key < 10)){
		
			for(i = 0; i < current_key; i++) {
				
				GPIOA->ODR |= GPIO_ODR_OD5;
				Delay(delay_button);
				GPIOA->ODR &= ~	GPIO_ODR_OD5;
				Delay(delay_button);
				
			}
			
		}
		
		if(current_key == 0x41){
		
			if (delay_button >= 200) {
				
				delay_button = delay_button - 100;
				
			}
			
		}
		
		if(current_key == 0x42){
		
			delay_button = 500;
			
		}
		
		if(current_key == 0x43){
		
			if (delay_button <= 900) {
				
				delay_button = delay_button + 100;
				
			}
			
		}
		
		current_key = 0x0;
		
	}
}
