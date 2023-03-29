;******************** (C) Yifeng ZHU *******************************************
; @file main.s
; @author  Yifeng Zhu
; @date Jan-17-2023
;*******************************************************************************


   INCLUDE core_cm4_constants.s     ; Load Constant Definitions
   INCLUDE stm32l476xx_constants.s     


; Green LED <--> PA.5
LED_PIN    EQU    5
BUTTON_PIN EQU 13
DELAY EQU 0x100050
MDELAY EQU 0x1000
HALFDELAY EQU 0x800


   AREA    main, CODE, READONLY
   EXPORT    __main             ; make __main visible to linker
   ENTRY           
   
__main    PROC
	
   BL pins
   
   LDR r0, = 360			;Input degrees
   LDR r1, =100
   MUL r0, r0, r1
   STR r0, [sp,#0x0C] 
   
   LDR r0, =71
   STR r0, [sp, #0x10] 	; Motor Ratio
   
  ;;;;;;;;;;;;;;;;;;;;;;START OF LOOP;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

loop

mode1
   LDR r0,[sp,#0x08]
   CMP r0, #0                       ;Checks current mode
   BEQ mode2        ;If solid, skips the toggle loop

;;;;;;;;;;;;;MY CODE;;;;;;;;;;;;;;;;;;
   
flash
   BL delay

   BL toggle_led
   
   LDR r0, =GPIOC_BASE              ;Check for button release
   LDR r1, [r0, #GPIO_IDR]
   AND r1, r1, #GPIO_IDR_IDR_13
   CMP r1, #0                       ;High if released
   BEQ mode2
   
   B flash
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

mode2
   LDR r0, = 1
   STR r0, [sp,#0x08]                       ;Swaps mode from press

   BL button_press

   BL button_release

;;;;;;;;;;;;;MY CODE;;;;;;;;;;;;;;;;;;

   BL toggle_led
   
   BL full_step
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
   B loop                           ;Goes to top of loop to start again
  
   ENDP
	   
full_step PROC

   LDR r3, =0          ;Loop Count

   
fullrepeat

   LDR r0, =GPIOC_BASE
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =MDELAY                  ;Delay for 1
adelay
   SUBS r1, r1, #1
   BNE adelay
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =MDELAY                  ;Delay for 2
bdelay
   SUBS r1, r1, #1
   BNE bdelay
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =MDELAY                  ;Delay for 3
cdelay
   SUBS r1, r1, #1
   BNE cdelay
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =MDELAY                  ;Delay for blink4
ddelay
   SUBS r1, r1, #1
   BNE ddelay
  
   ADD r3, r3, #1 ;Loop iterate
   LDR r1, [sp,#0x10] ;Motor Ratio
   LDR r2, [sp,#0x0C]
   
   MUL r1, r3, r1
   CMP r1, r2
   BLT fullrepeat
  
   BX     LR
   ENDP

half_step PROC
  
  LDR r3, =0          ;Loop Count

   
halfrepeat
  
   LDR r0, =GPIOC_BASE
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for 1
hadelay
   SUBS r1, r1, #1
   BNE hadelay
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for 2
hbdelay
   SUBS r1, r1, #1
   BNE hbdelay
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for 3
hcdelay
   SUBS r1, r1, #1
   BNE hcdelay
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for blink4
hddelay
   SUBS r1, r1, #1
   BNE hddelay
  
   LDR r0, =GPIOC_BASE
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for 5
hedelay
   SUBS r1, r1, #1
   BNE hedelay
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for 6
hfdelay
   SUBS r1, r1, #1
   BNE hfdelay
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for 7
hgdelay
   SUBS r1, r1, #1
   BNE hgdelay
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_6
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   BIC r1, r1, #GPIO_ODR_ODR_8
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, [r0, #GPIO_ODR]
   ORR r1, r1, #GPIO_ODR_ODR_9
   STR r1, [r0, #GPIO_ODR]
  
   LDR r1, =HALFDELAY                   ;Delay for blink8
hhdelay
   SUBS r1, r1, #1
   BNE hhdelay
  
   ADD r3, r3, #1 ;Loop iterate
   LDR r1, [sp,#0x10] ;Motor Ratio
   LDR r2, [sp,#0x0C]
   
   MUL r1, r3, r1
   CMP r1, r2
   BLT halfrepeat
  
   BX     LR
   ENDP

	   
button_press PROC
	
buttonstart
   LDR r0, =GPIOC_BASE              ;Check for button press
   LDR r1, [r0, #GPIO_IDR]
   AND r1, r1, #GPIO_IDR_IDR_13
   CMP r1, #GPIO_IDR_IDR_13     ;Low if pressed
   BEQ buttonstart
	
   BX LR
   ENDP
	   
button_release PROC
	
buttonstart1
   LDR r0, =GPIOC_BASE              ;Check for button release
   LDR r1, [r0, #GPIO_IDR]
   AND r1, r1, #GPIO_IDR_IDR_13
   CMP r1, #0                       ;High if released
   BEQ buttonstart1
	
   BX LR
   ENDP
	   
delay PROC
	
   LDR r1, =DELAY                   ;Delay for blink
delay1
   SUBS r1, r1, #1
   BNE delay1
	
	BX LR
	ENDP
	 
toggle_led PROC
	
   LDR r0, =GPIOA_BASE              ;Toggles LED on
   LDR r1, [r0, #GPIO_ODR]
   EOR r1, r1, #GPIO_ODR_ODR_5
   STR r1, [r0, #GPIO_ODR]
	
	BX LR
	ENDP

pins PROC
	
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
   ;LED pin assignments
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
   
   ;Motor pin assignments
   ;GPIO Mode:
   LDR r1, [r0, #GPIO_MODER]
   BIC r1, r1, #GPIO_MODER_MODER5
   ORR r1, r1, #GPIO_MODER_MODER5_0
   STR r1, [r0, #GPIO_MODER]
  
   LDR r1, [r0, #GPIO_MODER]
   BIC r1, r1, #GPIO_MODER_MODER8
   ORR r1, r1, #GPIO_MODER_MODER8_0
   STR r1, [r0, #GPIO_MODER]
  
   LDR r1, [r0, #GPIO_MODER]
   BIC r1, r1, #GPIO_MODER_MODER6
   ORR r1, r1, #GPIO_MODER_MODER6_0
   STR r1, [r0, #GPIO_MODER]
  
   LDR r1, [r0, #GPIO_MODER]
   BIC r1, r1, #GPIO_MODER_MODER9
   ORR r1, r1, #GPIO_MODER_MODER9_0
   STR r1, [r0, #GPIO_MODER]
  
   ;GPIO Speed
   LDR r1, [r0, #GPIO_OSPEEDR]
   BIC r1, r1, #GPIO_OSPEEDER_OSPEEDR5
   ORR r1, r1, #GPIO_OSPEEDER_OSPEEDR5_1
   STR r1, [r0, #GPIO_OSPEEDR]
  
   LDR r1, [r0, #GPIO_OSPEEDR]
   BIC r1, r1, #GPIO_OSPEEDER_OSPEEDR8
   ORR r1, r1, #GPIO_OSPEEDER_OSPEEDR8_1
   STR r1, [r0, #GPIO_OSPEEDR]
  
   LDR r1, [r0, #GPIO_OSPEEDR]
   BIC r1, r1, #GPIO_OSPEEDER_OSPEEDR6
   ORR r1, r1, #GPIO_OSPEEDER_OSPEEDR6_1
   STR r1, [r0, #GPIO_OSPEEDR]
  
   LDR r1, [r0, #GPIO_OSPEEDR]
   BIC r1, r1, #GPIO_OSPEEDER_OSPEEDR9
   ORR r1, r1, #GPIO_OSPEEDER_OSPEEDR9_1
   STR r1, [r0, #GPIO_OSPEEDR]
  
   ;GPIO Output Type
   LDR r1, [r0, #GPIO_OTYPER]
   BIC r1, r1, #GPIO_OTYPER_OT_5
   STR r1, [r0, #GPIO_OTYPER]
  
   LDR r1, [r0, #GPIO_OTYPER]
   BIC r1, r1, #GPIO_OTYPER_OT_8
   STR r1, [r0, #GPIO_OTYPER]
  
   LDR r1, [r0, #GPIO_OTYPER]
   BIC r1, r1, #GPIO_OTYPER_OT_6
   STR r1, [r0, #GPIO_OTYPER]
  
   LDR r1, [r0, #GPIO_OTYPER]
   BIC r1, r1, #GPIO_OTYPER_OT_9
   STR r1, [r0, #GPIO_OTYPER]
  
   ;GPIO Push-Pull
   LDR r1, [r0, #GPIO_PUPDR]
   BIC r1, r1, #GPIO_PUPDR_PUPDR5
   STR r1, [r0, #GPIO_PUPDR]
  
   LDR r1, [r0, #GPIO_PUPDR]
   BIC r1, r1, #GPIO_PUPDR_PUPDR8
   STR r1, [r0, #GPIO_PUPDR]
  
   LDR r1, [r0, #GPIO_PUPDR]
   BIC r1, r1, #GPIO_PUPDR_PUPDR6
   STR r1, [r0, #GPIO_PUPDR]
  
   LDR r1, [r0, #GPIO_PUPDR]
   BIC r1, r1, #GPIO_PUPDR_PUPDR9
   STR r1, [r0, #GPIO_PUPDR]

	
   BX LR
   ENDP

   END
               
   ALIGN           


   PRESERVE8
   AREA    myData, DATA, READWRITE
   ALIGN
array    DCD   1, 2, 3, 4
   END
