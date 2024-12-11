/*
 * global.h
 *
 *  Created on: Dec 6, 2024
 *      Author: User
 */
#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "stm32f1xx_hal.h"
#include "main.h"

#define MAX_BUFFER_SIZE 20

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

extern uint8_t rx_data;
extern uint8_t tx_data[50];
extern uint8_t end_data[10];
extern uint8_t temp;
extern uint8_t index_buffer;
extern uint8_t buffer_flag;
extern uint8_t command_flag; // 0: No command, 1: !RST#, 2: !OK#
extern uint8_t buffer[MAX_BUFFER_SIZE];

extern char response[30]; // Buffer to hold the response string

extern uint32_t ADC_value;
extern float voltage;

#endif /* INC_GLOBAL_H_ */
