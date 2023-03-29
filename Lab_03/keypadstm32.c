// defines for DRIVE_KEYPAD_ROW(row) and READ_KEYPAD_COL(col) here
#define LEDS GPIOC->ODR
#define DRIVE_KEYPAD_ROW(row) (GPIOB->MODER = 0x01 << (4+(row))*2)
#define READ_KEYPAD_COL(col) (GPIOB->IDR >> colmap[col] & 0x01)

/* USER CODE BEGIN PV */
//ECE177 -- used for getkey()
unsigned char colmap[] = {8, 9, 10, 12};

signed char getkey(void);			// forward declarations
signed char mapkey(signed char key);
void delaycycles(int cycles);
void mydelayms(unsigned int ms);
unsigned char serialRead(void);
void serialWrite(unsigned char c); 


int main(void)
{

    GPIOB->OTYPER = 0x0000;		// GPIOB (Keypad) pins, when outputs, are "push-pull" type
	GPIOB->MODER = 0x0000;	// All bits of GPIOB are initially inputs
	GPIOB->ODR = 0xFFFF;		// When GPIOB (keypad row) bits are output, the are logic 1

	unsigned char keyyy;

  while (1)
  {
	  keyyy=mapkey(getkey());
	  if(keyyy==1){
		  playsong(&steinsong);
	  }else if (keyyy==2){
		  playsong(&marylamb);
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//ECE177 Put your functions here (finish them up)

signed char getkey(void) {
// This routine will get a key on the keypad and will return the key number
// below, or -1 if no key is pressed.
// Keys are numbered top to bottom and left to right and the keypad is mapped
// as follows

//             GPIO->ODR
//             Output	Key                 KEY
//             Bits     Number            LABELING
//              4 |  0  1  2  3        | 1  2  3  A
//              5 |  4  5  6  7        | 4  5  6  B
//              6 |  8  9 10 11        | 7  8  9  C
//              7 | 12 13 14 15        | *  0  #  D
//                 ------------         ------------
//                   8  9 10 12
//             GPIO->IDR Input Bits

// Your code here
	unsigned char k[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	    HAL_Delay(20);
	    for(int r=0; r<4; r++){
	        DRIVE_KEYPAD_ROW(r);
	        nop();
	    for(int c=0; c<4; c++){
	        if(READ_KEYPAD_COL(c)==1){
	            return k[4*r+c];
	        }
	    }
	    }
	        return -1;
}

signed char mapkey(signed char key) {
//	Return the mapping of the given key according to the following

//             GPIO->ODR
//             Output	Key                 KEY
//             Bits     Number            LABELING
//              4 |  0  1  2  3        | 1  2  3  A
//              5 |  4  5  6  7        | 4  5  6  B
//              6 |  8  9 10 11        | 7  8  9  C
//              7 | 12 13 14 15        | *  0  #  D
//                 ------------         ------------
//                   8  9 10 12
//             GPIO->IDR Input Bits

// Your code here
	signed char k[]={1,2,3,10,4,5,6,11,7,8,9,12,'*',0,'#',13};
	    if(key<0||key>15){
	        return -1;
	    }
	return k[key];
}