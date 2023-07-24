;******************** (C) Yifeng ZHU *******************************************
; @file    main.s
; @author  Yifeng Zhu
; @date    Jan-17-2023
;*******************************************************************************

	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s      

; Green LED <--> PA.5
LED_PIN	EQU	5
BUTTON_PIN EQU 13

	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	IMPORT screen_init
	IMPORT counter
	ENTRY			
				
__main	PROC

    ; Enable the clock to GPIO Port A	
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOAEN
	STR r1, [r0, #RCC_AHB2ENR]
	
	; Enable the clock to GPIO Port C
	
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOCEN
	STR r1, [r0, #RCC_AHB2ENR]
	
	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOA_BASE
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #GPIO_MODER_MODER5
	ORR r1, r1, #GPIO_MODER_MODER5_0
	STR r1, [r0, #GPIO_MODER]
	
	;Button pin assignments
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #GPIO_MODER_MODER13
	STR r1, [r0, #GPIO_MODER]
	
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #GPIO_PUPDR_PUPDR13
	STR r1, [r0, #GPIO_PUPDR]
	
	;Initialise display
	BL screen_init

;Start of forever loop
loop

	LDR r0, =GPIOC_BASE				;Check for button press
	LDR r1, [r0, #GPIO_IDR]
	AND r1, r1, #GPIO_IDR_IDR_13
	CMP r1, #GPIO_IDR_IDR_13		;Low if pressed
	BEQ loop
loop1
	LDR r0, =GPIOC_BASE				;Check for button release
	LDR r1, [r0, #GPIO_IDR]
	AND r1, r1, #GPIO_IDR_IDR_13
	CMP r1, #0						;High if released
	BEQ loop1
	
	LDR r0, =GPIOA_BASE				;Toggles LED on/off
	LDR r1, [r0, #GPIO_ODR]
	EOR r1, r1, #GPIO_ODR_ODR_5
	STR r1, [r0, #GPIO_ODR]
	
	;Call counter function that adds 1 and displays the count_hold variable
	BL counter
	
	B loop							;Goes to top of loop to start again
  
	ENDP
					
	ALIGN			

	AREA    myData, DATA, READWRITE
	EXPORT count_hold
	ALIGN
count_hold	DCD   0
	END