#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "keypad.h"

// PC.13 <--> Blue user button
#define BUTTON_PIN 13

void I2C_GPIO_init(void);

void configure_MOTOR_BUTTON_pin(){
  // Enable the clock to GPIO Port C	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;  
	
	//Motor
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
	
	//Button
	//Sets the GPIO mode to input (00), no value line required after
	GPIOC->MODER &= ~GPIO_MODER_MODE13; 

	//Sets GPIOC to no pull-up, no pull-down
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13; 
	
}

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
	configure_MOTOR_BUTTON_pin();
	configure_KEYPAD_pin();
	
	//OLED
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	
	//For loop delay variables
	int delay = 8999; 
	int delay_iterator = 0;
	
	int loop = 0; 															 //Current loop
	
	//Keypad Variables
	char col, pressed_col, row, pressed_row = 0; //Keeps track of iteration and which button is pressed
	int cols[] = {4, 10, 11, 12};								 //Column GPIO Pins
	int rows[] = {0, 1, 2, 3};									 //Row GPIO Pins
	int input_mask = 0x1C10;										 //For masking everything but input pins
	int output_to_one = 0xF;										 //For setting rows bit to 1
	char COL_ROW_DEBUG[64] = "";								 //Holds COL and ROW data for OLED
	char key_map [4][4] = {											 //Maps keys to each col and row
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'},
	};
	
	char current_key = 0xFF;										//Holds the current key in ASCII form
	char inputted_pin[64] = "";									//Holds key presses up to 6 for OLED
	char passkey[6] = "123456";									//passkey for spinning the motor
	char debug_passkey[6] = "654321";						//passkey to enable debug mode
	int correct_pin_check = 0;									//Counts how many entered digits are right for motor
	int debug_pin_check = 0;										//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ debug
	int correct_passkey = 0;										//0 for incorrect, 1 for correct
	int debug_mode = 0;													//0 for regular, 1 for debug mode
	int i = 0;																	//Iterator
	
	char message[64] = "Ready!"; 								// Start message
	char strleng[2];														//Length of user inputted pin
	float current_angle = 0;										//Calculated angle
	char print_angle[64] = "";									//Stepper angle for OLED
	
	GPIOC->ODR &= ~GPIO_ODR_OD5;
	GPIOC->ODR &= ~GPIO_ODR_OD6;
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9; 								//Initialize motor pins to zero
	
	ssd1306_Init();
	ssd1306_Fill(White);
	ssd1306_SetCursor(2,0);
	ssd1306_WriteString(message, Font_11x18, Black);		//Displays "Ready" on display once everything has been initiated
	ssd1306_UpdateScreen();
	
	while(1){
		
		float stepper_ratio = 1.4361111; //517 loops starting at 0
		float degrees = 15; //Sets how far the motor turns with a buttom press
		int full_half = 0; //Full stepping set 0, Half step set 1

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
		for(delay_iterator = 0; delay_iterator<1000; delay_iterator++);
		
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
		strncat(inputted_pin, &current_key, 1);								//Appends key to string displayed on OLED
		
		//Passkey checking
		if((strlen(inputted_pin)) == 6){											//Once the inputted pin reaches 6...
			for(i = 0; i < 6; i++){															//...automatically checks against passkey
				if(inputted_pin[i] == passkey[i]) {								//Checks each element
					correct_pin_check++;														//If the corresponding element matches adds one
				} else if(inputted_pin[i] == debug_passkey[i]) {	//Debug mode passkey checking
					debug_pin_check++;
				}
			}
			if(correct_pin_check == 6){ 			//Once correct enables the motor functionality and resets...
				correct_passkey = 1;						//...so the passkey has to be entered again
			} else if(debug_pin_check == 6){  //Enables debug mode if correct
				debug_mode++;
			}
			strcpy(inputted_pin, "");					//Resets input
			
			correct_pin_check = 0;						//Resets password tracker values
			debug_pin_check = 0;
		}
		
		ssd1306_Fill(White);
		ssd1306_SetCursor(2,40);
		ssd1306_WriteString(inputted_pin, Font_11x18, Black);
		
		if((debug_mode % 2) == 1) {
			sprintf(COL_ROW_DEBUG, "%d COL %d ROW", pressed_col, pressed_row);
			ssd1306_SetCursor(2,20);
			ssd1306_WriteString(COL_ROW_DEBUG, Font_11x18, Black);//Displays current keypad col and row
		}
		
		
		ssd1306_UpdateScreen();																//Updates OLED every time a button is pressed
		
		//Runs motor when passkey is correct
		if(correct_passkey == 1) {
			
			for(loop = 0;loop < (degrees*stepper_ratio); loop++) { 		//Loops until set degree is reached
				if((debug_mode % 2) == 1){																//Debug code
					sprintf(message, "%d Lp %f ", loop, current_angle);
					//sprintf(COL_ROW_DEBUG, "%d COL %d ROW", pressed_col, pressed_row);
					sprintf(strleng, "%d", (strlen(inputted_pin)));				//Converts variables into strings
					ssd1306_SetCursor(2,60);
					ssd1306_WriteString(strleng, Font_11x18, Black);			//Display string length
					ssd1306_SetCursor(2,20);
					ssd1306_WriteString(COL_ROW_DEBUG, Font_11x18, Black);//Displays current keypad col and row
					ssd1306_Fill(White);
					
					ssd1306_DrawCircle(120, 2, 2, Black); 								//Degree symbol
				} else {																								//Non Debug mode
					sprintf(message, "%f", current_angle);								//Current abgle
					ssd1306_DrawCircle(120, 2, 2, Black); 								//Degree symbol
				}

				ssd1306_SetCursor(2,0);
				ssd1306_WriteString(message, Font_11x18, Black);  		//Displays current loop #
				ssd1306_UpdateScreen();	
				current_angle = (loop+2)/stepper_ratio; //Calculates current angle
				
				
				if(full_half == 0) { // Calls full step or half step function based on setting above
					full_step(delay_iterator, delay);
				} else {
					half_step(delay_iterator, delay);
				}
			
			}
			correct_passkey = 0; //Resets so passkey has to be reentered to spin motor
		}
		
		for(delay_iterator = 0; delay_iterator<1900000; delay_iterator++); //Delay between repeated characters
		
	}

}
