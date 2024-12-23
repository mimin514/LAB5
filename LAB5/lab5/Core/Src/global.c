/*
 * global.c
 *
 *  Created on: Dec 6, 2024
 *      Author: User
 */


#include "global.h"

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim2;

uint8_t rx_data;
uint8_t tx_data[50] = "START: !RST#\r\nEND: !OK#\r\n--------------------\r\n";
uint8_t end_data[10] = "end\r\n";
uint8_t temp = 0;
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t command_flag = 0; // 0: No command, 1: !RST#, 2: !OK#
uint8_t buffer[MAX_BUFFER_SIZE] = {0};

char response[30]; // Buffer to hold the response string

uint32_t ADC_value = 0;
float voltage = 0;
