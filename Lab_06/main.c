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

void I2C_GPIO_init(void);

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
	
	//OLED
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	
	char message[64] = "Ready!"; // Start message
	
	ssd1306_Init();
	ssd1306_Fill(White);
	ssd1306_SetCursor(2,0);
	ssd1306_WriteString(message, Font_11x18, Black);		//Displays "Ready" on display once everything has been initiated
	ssd1306_UpdateScreen();

	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();
	
	NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable Interrupt
 
  // Connect External Line to the GPI
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
  SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC;
 
  // Interrupt Mask Register
  // 0 = marked, 1 = not masked (enabled)
  EXTI->IMR1  |= EXTI_IMR1_IM13; 	
 
  // Rising trigger selection
  // 0 = trigger disabled, 1 = trigger enabled
  EXTI->RTSR1 |= EXTI_RTSR1_RT13; 

	
  // Dead loop & program hangs here
	while(1){
		
		
		
	}
}

void LED_status(void) { //Checks current LED status and displays the corresponding status on the display
	
	if(GPIOA->ODR & GPIO_ODR_OD5) {//Checks if the LED is on 
		
			ssd1306_Fill(White);
			ssd1306_SetCursor(2,0);
			ssd1306_WriteString("LED is ON", Font_11x18, Black);
			ssd1306_UpdateScreen();
			
		} else {										//If the LED is off it will display that it is off
			
			ssd1306_Fill(White);
			ssd1306_SetCursor(2,0);
			ssd1306_WriteString("LED is OFF", Font_11x18, Black);
			ssd1306_UpdateScreen();
			
		}
	
}

void EXTI15_10_IRQHandler(void) {
	
	if((EXTI->PR1 & EXTI_PR1_PIF13) != 0){ //Checks pin 13 specifically
		
		GPIOA->ODR ^= GPIO_ODR_OD5; 	//Toggles LED status
		
		LED_status();												//Displays LED status on the display
		
		EXTI->PR1 |= EXTI_PR1_PIF13;				//Toggles the flag to exit the interrupt.
	}
	
}
