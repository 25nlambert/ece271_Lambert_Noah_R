#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

void configure_LED_pin(){
  // Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);      // Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
	GPIOA->OSPEEDR |=   2<<(2*LED_PIN);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
	
	//Part B Start
	//Enable the clock to GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; 

	//Sets the GPIO mode to input (00), no value line required after
	GPIOC->MODER &= ~(3UL<<(2*BUTTON_PIN)); 

	//Sets GPIOC to no pull-up, no pull-down
	GPIOC->PUPDR &= ~(3<<(2*BUTTON_PIN)); 
	//Part B End
	
}

int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();	
	
	while(1){
		
		int i,j = 0;
		
		while((GPIOC->IDR & GPIO_IDR_IDR_13));
		while(!(GPIOC->IDR & GPIO_IDR_IDR_13)); //Checks for button press
		j++;
		while(j != 0){ 														//Toggle LED loop
			for(i = 0; i<999999; i++); 							//Wait before toggle
			GPIOA->ODR ^= 1 << LED_PIN; 						//Toggle LED
			if(!(GPIOC->IDR & GPIO_IDR_IDR_13)) j=0;//Exits loop if button is pressed.
		}
		
		while((GPIOC->IDR & GPIO_IDR_IDR_13));
		while(!(GPIOC->IDR & GPIO_IDR_IDR_13));//Checks for button press
		GPIOA->ODR |= 1 << LED_PIN;// Sets LED to on state
		
	}
	
}

