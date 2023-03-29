#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"

// PC.13 <--> Blue user button
#define BUTTON_PIN 13

void I2C_GPIO_init(void);

void configure_LED_pin(){
  // Enable the clock to GPIO Port C	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;  
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~GPIO_MODER_MODE5;  
	GPIOC->MODER |=  GPIO_MODER_MODE5_0;      //  PC5 Output(01)
	
	GPIOC->MODER &= ~GPIO_MODER_MODE8;  
	GPIOC->MODER |=  GPIO_MODER_MODE8_0;      //  PC8
	
	GPIOC->MODER &= ~GPIO_MODER_MODE6;  
	GPIOC->MODER |=  GPIO_MODER_MODE6_0;      //  PC6
	
	GPIOC->MODER &= ~GPIO_MODER_MODE9;  
	GPIOC->MODER |=  GPIO_MODER_MODE9_0;      //  PC9
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;
	GPIOC->OSPEEDR |=  GPIO_OSPEEDR_OSPEED5_1;  // PC5 Fast speed
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8;
	GPIOC->OSPEEDR |=  GPIO_OSPEEDR_OSPEED8_1;  // PC8
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6;
	GPIOC->OSPEEDR |=  GPIO_OSPEEDR_OSPEED6_1;  // PC6
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9;
	GPIOC->OSPEEDR |=  GPIO_OSPEEDR_OSPEED9_1;  // PC9
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT5;      // PC5 Push-pull
	
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT8;      // PC8
	
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT6;      // PC6
	
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT9;      // PC9
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD5;  // PC5 No pull-up, no pull-down
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD8;  // PC8
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD6;  // PC6
	
	GPIOC->PUPDR  &= ~GPIO_PUPDR_PUPD9;  // PC9
	
	//Sets the GPIO mode to input (00), no value line required after
	GPIOC->MODER &= ~GPIO_MODER_MODE13; 

	//Sets GPIOC to no pull-up, no pull-down
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13; 
	
}

void full_step(int delay_iterator, int delay) {
	GPIOC->ODR |= GPIO_ODR_OD5;
	GPIOC->ODR &= ~GPIO_ODR_OD6;
	//GPIOC->ODR &= ~GPIO_ODR_OD8;
	//GPIOC->ODR |= GPIO_ODR_OD9;
	
	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //1
	
	//GPIOC->ODR |= GPIO_ODR_OD5;
	//GPIOC->ODR &= ~GPIO_ODR_OD6;
	GPIOC->ODR |= GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //2
	
	GPIOC->ODR &= ~GPIO_ODR_OD5;
	GPIOC->ODR |= GPIO_ODR_OD6;
	//GPIOC->ODR |= GPIO_ODR_OD8;
	//GPIOC->ODR &= ~GPIO_ODR_OD9;
	
	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //3
	
	//GPIOC->ODR &= ~GPIO_ODR_OD5;
	//GPIOC->ODR |= GPIO_ODR_OD6;
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //4
}

void half_step(int delay_iterator, int delay) {
	
	delay = 6799;
	
	GPIOC->ODR |= GPIO_ODR_OD5;
	GPIOC->ODR &= ~GPIO_ODR_OD6;
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //1
	
	//GPIOC->ODR |= GPIO_ODR_OD5;
	//GPIOC->ODR &= ~GPIO_ODR_OD6;
	//GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //2
	
	//GPIOC->ODR |= GPIO_ODR_OD5;
	//GPIOC->ODR &= ~GPIO_ODR_OD6;
	GPIOC->ODR |= GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //3
	
	GPIOC->ODR &= ~GPIO_ODR_OD5;
	//GPIOC->ODR &= ~GPIO_ODR_OD6;
	//GPIOC->ODR |= GPIO_ODR_OD8;
	//GPIOC->ODR &= ~GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //4
	
	//GPIOC->ODR &= ~GPIO_ODR_OD5;
	//GPIOC->ODR |= GPIO_ODR_OD6;
	GPIOC->ODR |= GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //5
	
	GPIOC->ODR &= ~GPIO_ODR_OD5;
	GPIOC->ODR |= GPIO_ODR_OD6;
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //6
	
	GPIOC->ODR &= ~GPIO_ODR_OD5;
	GPIOC->ODR |= GPIO_ODR_OD6;
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //7
	
	GPIOC->ODR &= ~GPIO_ODR_OD5;
	GPIOC->ODR &= ~GPIO_ODR_OD6;
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;

	for(delay_iterator = 0; delay_iterator<delay; delay_iterator++); //8
}

int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();
	
	//OLED
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	
	char message[64] = "Ready!"; // Start message
	char print_angle[64] = "";
	int delay = 8999; 
	int delay_iterator = 0;
	float current_angle = 0;
	int loop = 0; //Current loop
	float stepper_ratio = 1.4361111; //517 loops starting at 0
	
	float degrees = 360; //Sets how far the motor turns with a buttomn press
	int full_half = 0; //Full stepping set 0, Half step set 1
	
	ssd1306_Init();
	ssd1306_Fill(White);
	ssd1306_SetCursor(2,0);
	ssd1306_WriteString(message, Font_11x18, Black);		//Displays "Ready" on display once everything has been initiated
	ssd1306_UpdateScreen();

	GPIOC->ODR &= ~GPIO_ODR_OD5;
	GPIOC->ODR &= ~GPIO_ODR_OD6;
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9; //Resets all pins to zero
	
	while(1){
		
		while((GPIOC->IDR & GPIO_IDR_IDR_13));
		while(!(GPIOC->IDR & GPIO_IDR_IDR_13)); //Checks for button press
		
		for(loop = 0;loop < (degrees*stepper_ratio); loop++) { //Loops until set degree is reached
			
			sprintf(message, "%d Loops ", loop);
			sprintf(print_angle, "Angle %f", current_angle);
			ssd1306_Fill(White);
			ssd1306_SetCursor(2,0);
			ssd1306_WriteString(message, Font_11x18, Black);  		//Displays current loop #
			ssd1306_SetCursor(2,20);
			ssd1306_WriteString(print_angle, Font_11x18, Black);  //Displays current angle
			ssd1306_DrawCircle(100, 20, 2, Black); 								//Displays degree symbol
			ssd1306_UpdateScreen();	
			current_angle = (loop+2)/stepper_ratio; //Calculates current angle
			
			
			if(full_half == 0) { // Calls full step or half step function based on setting above
				full_step(delay_iterator, delay);
			} else {
				half_step(delay_iterator, delay);
			}
			
		}
		
	}
	
}
