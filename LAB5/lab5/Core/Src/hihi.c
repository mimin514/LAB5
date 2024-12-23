///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file           : main.c
//  * @brief          : Main program body
//  ******************************************************************************
//  */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
//#include "main.h"
//
///* Private includes ----------------------------------------------------------*/
///* USER CODE BEGIN Includes */
//#include <stdio.h>
//#include <string.h>
///* USER CODE END Includes */
//
///* Private typedef -----------------------------------------------------------*/
///* USER CODE BEGIN PTD */
//
///* USER CODE END PTD */
//
///* Private define ------------------------------------------------------------*/
///* USER CODE BEGIN PD */
///* USER CODE END PD */
//
///* Private macro -------------------------------------------------------------*/
///* USER CODE BEGIN PM */
//
///* USER CODE END PM */
//
///* Private variables ---------------------------------------------------------*/
//ADC_HandleTypeDef hadc1;
//
//UART_HandleTypeDef huart2;
//
///* USER CODE BEGIN PV */
//
///* USER CODE END PV */
//
///* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_ADC1_Init(void);
//static void MX_USART2_UART_Init(void);
///* USER CODE BEGIN PFP */
//
///* USER CODE END PFP */
//
///* Private user code ---------------------------------------------------------*/
///* USER CODE BEGIN 0 */
//uint8_t rx_data;
//uint8_t tx_data[50]="START: !RST#\r\nEND: !OK#\r\n--------------------\r\n";
//uint8_t end_data[10]="end\r\n";
//
//#define MAX_BUFFER_SIZE 30
//uint8_t temp = 0;
//uint8_t buffer[MAX_BUFFER_SIZE];
//uint8_t index_buffer = 0;
//uint8_t buffer_flag = 0;
//uint8_t command_flag = 0; // 0: No command, 1: !RST#, 2: !OK#
//char response[30]; // Buffer to hold the response string
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//
//    if (huart->Instance == USART2) {
//        buffer[index_buffer++] = temp;
//        if (index_buffer == MAX_BUFFER_SIZE) index_buffer = 0; // Prevent overflow
//        buffer_flag = 1;
//        HAL_UART_Receive_IT(&huart2, &temp, 1);
//    }
//}
//uint32_t ADC_value = 0;
//float voltage = 0;
//
//
//void command_parser_fsm() {
//    static uint8_t state = 0;
//
//    for (uint8_t i = 0; i < index_buffer; i++) {
//        switch (state) {
//            case 0:
//                if (buffer[i] == '!') state = 1;
//                break;
//            case 1:
//                if (buffer[i] == 'R') state = 2;
//                else if (buffer[i] == 'O') state = 5;
//                else state = 0;
//                break;
//            case 2:
//                if (buffer[i] == 'S') state = 3;
//                else state = 0;
//                break;
//            case 3:
//                if (buffer[i] == 'T') state = 4;
//                else state = 0;
//                break;
//            case 4:
//                if (buffer[i] == '#') {
//                    // Command received: !RST#
//                    command_flag = 1;
//                }
//                state = 0;
//                break;
//            case 5:
//                if (buffer[i] == 'K') state = 6;
//                else state = 0;
//                break;
//            case 6:
//                if (buffer[i] == '#') {
//                    // Command received: !OK#
//                    command_flag = 2;
//                }
//                state = 0;
//                break;
//            default:
//                state = 0;
//        }
//    }
//    index_buffer = 0; // Reset buffer after parsing
//}
//
//
//void uart_communiation_fsm() {
//    static uint8_t state = 0;
//    static uint32_t last_time = 0; // Lưu th�?i gian ch�? !OK#
//    uint32_t current_time = HAL_GetTick();
//
//    switch (state) {
//        case 0: // Ch�? lệnh !RST#
//            if (command_flag == 1) { // Khi nhận được lệnh !RST#
//                uint32_t ADC_value = 0;
//
//                // �?�?c giá trị ADC từ PA0
//                HAL_ADC_Start(&hadc1); // Bắt đầu chuyển đổi ADC
//                if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
//                    ADC_value = HAL_ADC_GetValue(&hadc1); // Lấy giá trị ADC
//                }
//                HAL_ADC_Stop(&hadc1); // Dừng ADC
//
//                // Gửi giá trị ADC qua UART
//                sprintf(response, "!ADC=%ld#\r\n", ADC_value);
//                HAL_UART_Transmit(&huart2, (uint8_t *)response, strlen(response), 100);
//
//                // Chuyển sang trạng thái ch�? !OK#
//                state = 1;
//                last_time = current_time; // Lưu th�?i gian hiện tại
//            }
//            break;
//
//        case 1: // Ch�? lệnh !OK#
//            if (command_flag == 2) { // Khi nhận được lệnh !OK#
//            	  HAL_UART_Transmit(&huart2,end_data, sizeof(end_data), 100);
//                state = 0; // Quay v�? trạng thái ch�? lệnh !RST#
//            } else if (current_time - last_time > 3000) { // Nếu hết 3 giây mà không nhận được !OK#
//                HAL_UART_Transmit(&huart2, (uint8_t *)response, strlen(response), 100); // Gửi lại giá trị ADC
//                last_time = current_time; // Cập nhật th�?i gian ch�?
//            }
//            break;
//
//        default:
//            state = 0;
//    }
//
//    // Reset c�? sau khi xử lý
//    command_flag = 0;
//}
//
///* USER CODE END 0 */
//
///**
//  * @brief  The application entry point.
//  * @retval int
//  */
//int main(void)
//{
//  /* USER CODE BEGIN 1 */
//
//  /* USER CODE END 1 */
//
//  /* MCU Configuration--------------------------------------------------------*/
//
//  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//  HAL_Init();
//
//  /* USER CODE BEGIN Init */
//
//  /* USER CODE END Init */
//
//  /* Configure the system clock */
//  SystemClock_Config();
//
//  /* USER CODE BEGIN SysInit */
//
//  /* USER CODE END SysInit */
//
//  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
//  MX_ADC1_Init();
//  MX_USART2_UART_Init();
//  /* USER CODE BEGIN 2 */
//  HAL_UART_Receive_IT(&huart2, &temp, 1);
//  HAL_UART_Transmit(&huart2,tx_data, sizeof(tx_data), 100);
//  /* USER CODE END 2 */
//
//  /* Infinite loop */
//  /* USER CODE BEGIN WHILE */
//    while (1) {
//    	if (buffer_flag) {
//    	            command_parser_fsm();
//    	            buffer_flag = 0;
//    	        }
//    	        uart_communiation_fsm();
//
//    /* USER CODE END WHILE */
//
//    /* USER CODE BEGIN 3 */
//    }
//  /* USER CODE END 3 */
//}
//
///**
//  * @brief System Clock Configuration
//  * @retval None
//  */
//void SystemClock_Config(void)
//{
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//
//  /** Initializes the RCC Oscillators according to the specified parameters
//  * in the RCC_OscInitTypeDef structure.
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Initializes the CPU, AHB and APB buses clocks
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
//  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}
//
///**
//  * @brief ADC1 Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_ADC1_Init(void)
//{
//
//  /* USER CODE BEGIN ADC1_Init 0 */
//
//  /* USER CODE END ADC1_Init 0 */
//
//  ADC_ChannelConfTypeDef sConfig = {0};
//
//  /* USER CODE BEGIN ADC1_Init 1 */
//
//  /* USER CODE END ADC1_Init 1 */
//
//  /** Common config
//  */
//  hadc1.Instance = ADC1;
//  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
//  hadc1.Init.ContinuousConvMode = ENABLE;
//  hadc1.Init.DiscontinuousConvMode = DISABLE;
//  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  hadc1.Init.NbrOfConversion = 1;
//  if (HAL_ADC_Init(&hadc1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure Regular Channel
//  */
//  sConfig.Channel = ADC_CHANNEL_0;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN ADC1_Init 2 */
//
//  /* USER CODE END ADC1_Init 2 */
//
//}
//
///**
//  * @brief USART2 Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_USART2_UART_Init(void)
//{
//
//  /* USER CODE BEGIN USART2_Init 0 */
//
//  /* USER CODE END USART2_Init 0 */
//
//  /* USER CODE BEGIN USART2_Init 1 */
//
//  /* USER CODE END USART2_Init 1 */
//  huart2.Instance = USART2;
//  huart2.Init.BaudRate = 9600;
//  huart2.Init.WordLength = UART_WORDLENGTH_8B;
//  huart2.Init.StopBits = UART_STOPBITS_1;
//  huart2.Init.Parity = UART_PARITY_NONE;
//  huart2.Init.Mode = UART_MODE_TX_RX;
//  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//  if (HAL_UART_Init(&huart2) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN USART2_Init 2 */
//
//  /* USER CODE END USART2_Init 2 */
//
//}
//
///**
//  * @brief GPIO Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_GPIO_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//
//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//
//  /*Configure GPIO pin : PA5 */
//  GPIO_InitStruct.Pin = GPIO_PIN_5;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//}
//
///* USER CODE BEGIN 4 */
//
///* USER CODE END 4 */
//
///**
//  * @brief  This function is executed in case of error occurrence.
//  * @retval None
//  */
//void Error_Handler(void)
//{
//  /* USER CODE BEGIN Error_Handler_Debug */
//  /* User can add his own implementation to report the HAL error return state */
//  __disable_irq();
//  while (1)
//  {
//  }
//  /* USER CODE END Error_Handler_Debug */
//}
//
//#ifdef  USE_FULL_ASSERT
///**
//  * @brief  Reports the name of the source file and the source line number
//  *         where the assert_param error has occurred.
//  * @param  file: pointer to the source file name
//  * @param  line: assert_param error line source number
//  * @retval None
//  */
//void assert_failed(uint8_t *file, uint32_t line)
//{
//  /* USER CODE BEGIN 6 */
//  /* User can add his own implementation to report the file name and line number,
//     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//  /* USER CODE END 6 */
//}
//#endif /* USE_FULL_ASSERT */
