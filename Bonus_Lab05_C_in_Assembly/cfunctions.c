#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"

extern int count_hold;

void screen_init(void){
	
	//Initialize clock and I2C
	System_Clock_Init();
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	
	//Initialise display, clear display and add in the start value
	ssd1306_Init();
	ssd1306_Fill(White);
	ssd1306_SetCursor(53,18);
	ssd1306_WriteString("0", Font_11x18, Black);
	ssd1306_UpdateScreen();
	
}

void counter(){
	
	//Iterate press counter
	count_hold++;
	
	//Initialise string for holding output message and convert the current press to string
	char message[64] = "Ready!"; // Start message
	sprintf(message, "%d",count_hold);
	
	//Clear display and display press count in ~middle of the screens
	ssd1306_Fill(White);
	ssd1306_SetCursor(53,18);
	ssd1306_WriteString(message, Font_11x18, Black);
	ssd1306_UpdateScreen();
	
}