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

void RTC_Clock_Enable(){
	
	if((RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) == 0) {
		//Enable clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
		
		//Delay
		(void) RCC->APB1ENR1;
	}
	
	//LSE for RTC clock source
	if((PWR->CR1 & PWR_CR1_DBP) == 0) {
		//Write access for RTC and registers in backup domain
		PWR->CR1 |= PWR_CR1_DBP;
		//Wait for the write protection to be enabled
		while((PWR->CR1 & PWR_CR1_DBP) == 0);
	}
	
	//RESET LSEON AND LSEBYP bits before config
	//BDCR is the backup domain control register
	RCC->BDCR &= ~(RCC_BDCR_LSEON | RCC_BDCR_LSEBYP); 
	
	//Select RTC clock is backup is reset
	RCC->BDCR |=  RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	
	//Wait LSE clock to be ready
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0) {
			RCC->BDCR |= RCC_BDCR_LSEON;
	}
	
	//LSE RTC clock source
	//RTCSE[1:0]  00 = No Clock, 01 = LSE, 10 = LSI, 11 = HSE
	RCC->BDCR &= ~RCC_BDCR_RTCSEL;
	RCC->BDCR |=  RCC_BDCR_RTCSEL_0;
	
	//Disable power interface clock
	RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;
	
	//Enable RTC clock
	RCC->BDCR |= RCC_BDCR_RTCEN;
	
}

void RTC_Init() {
	
	//Enable RTC clock
	RTC_Clock_Enable();
	
	//Disable write protection 
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	
	//Enter initialization mode to program TR and DR registers
	RTC->ISR |= RTC_ISR_INIT;
	
	//Wait for initilazation mode
	while((RTC->ISR & RTC_ISR_INITF) == 0);
	
	//Hour format: 0 = 24 hour/day; 1 = am/pm
	RTC->CR &= ~RTC_CR_FMT;
	
	//Generate the 1Hz clock for RTC time counter
	RTC->PRER = ((127 << 16) | 255);
	
	//Set Time
	RTC->TR = 0U<<22 | 2U<<20 | 3U<<16 | 5U<<12 | 9U<<8 | 5U<<4;
	
	//Set Date
	RTC->DR = 1U << 20 | 8U << 16 | 1U << 13 | 1U << 12 | 2U << 8 | 3U << 4 | 1U;
	
	//Exit initialization
	RTC->ISR &= ~RTC_ISR_INIT;
	
	//Enable write protection for RTC
	RTC->WPR = 0xFF;
	
}

int main(void){
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	
	//Oled 
	ssd1306_Init();
	
	//RTC
	RTC_Init();
	
	//Holding time and date data
	int seconds = 0;
	int minutes = 0;
	int hours = 0;
	
	int year = 0;
	int month = 0;
	int day = 0;
	int weekday = 0;

	int pm = 0; //AM or PM

	char pm_char[10] = "Ready!"; // AM/PM start message
	char time[64] = "Ready!"; // Time start message
	char date[64] = "Ready!"; // Date start message
	char cur_weekday[64] = "Ready!"; // Current weekday start message

	// Dead loop & program hangs here
	while(1){
		
		//Time calculations
		seconds = ((RTC->TR & 0xF)        + ((RTC->TR >> 4) & 0x7) * 10); 		//Finds the ones place and adds to the tens for each part
		minutes = ((RTC->TR >>  8) & 0xF) + (((RTC->TR >> 12) & 0x7) * 10);	
		hours   = ((RTC->TR >> 16) & 0xF) + (((RTC->TR >> 20) & 0x3) * 10);
		
		//24 Hour to 12 Hour
		if((hours > 12)) {
			hours = hours - 12;
			pm = 1;
		} else {
			pm = 0;
			if(hours == 0) {//So 12am is not 00am
				hours = hours + 12;
			}
		}
		
		//Date and weekday calculations
		year    = ((RTC->DR >> 16) & 0xF) + (((RTC->DR >> 20) & 0xF) * 10) + 2000;
		month = ((RTC->DR >> 8) & 0xF) + (((RTC->DR >> 12) & 0x1) * 10);
		day     = ((RTC->DR) & 0xF) + (((RTC->DR >> 4) & 0x03) * 10);
		
		weekday = ((RTC->DR >> 13) & 0x7);
		
		//Connect the string holding the day to the weekday variable (Monday 001- Sunday 111)
		switch(weekday) {
      case 1 :
				sprintf(cur_weekday, "Monday");
        break;
			case 2 :
				sprintf(cur_weekday, "Tuesday");
        break;
			case 3 :
				sprintf(cur_weekday, "Wednesday");
        break;
			case 4 :
				sprintf(cur_weekday, "Thursday");
        break;
			case 5 :
				sprintf(cur_weekday, "Friday");
        break;
			case 6 :
				sprintf(cur_weekday, "Saturday");
        break;
			case 7 :
				sprintf(cur_weekday, "Sunday");
        break;
		}
		
		switch(pm) {//Changes the AM PM string accordingly
			case 0 :
				sprintf(pm_char, "AM");
				break;
			case 1 :
				sprintf(pm_char, "PM");
				break;
		}
		
		//Clear display
		ssd1306_Fill(White);
		
		//Print weekday
		ssd1306_SetCursor(2,40);
		ssd1306_WriteString(cur_weekday, Font_11x18, Black);
		
		//Print time
		sprintf(time, "%02d:%02d:%02d ", hours, minutes, seconds);
		strncat(time, pm_char, 2);
		ssd1306_SetCursor(2,0);
		ssd1306_WriteString(time, Font_11x18, Black);
		
		//Print Date
		sprintf(date, "%02d/%02d/%d", month, day, year);
		ssd1306_SetCursor(2,20);
		ssd1306_WriteString(date, Font_11x18, Black);
		
		//Display changes on OLED
		ssd1306_UpdateScreen();
		
	}
	
}
