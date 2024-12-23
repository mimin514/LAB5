/*
 * fsm_uart.c
 *
 *  Created on: Dec 6, 2024
 *      Author: User
 */

#include "main.h"
#include "fsm_uart.h"
#include "global.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>

void uart_communiation_fsm() {
    static uint8_t state = 0;


    switch (state) {
        case 0: // Chờ  !RST#
            if (command_flag == 1) { // Khi nhận được lệnh !RST#
                HAL_ADC_Start(&hadc1); // Bắt đầu chuyển đổi ADC
                ADC_value = HAL_ADC_GetValue(&hadc1); // Lấy giá trị ADC
                HAL_ADC_Stop(&hadc1); // Dừng ADC

                sprintf(response, "!ADC=%ld#\r\n", ADC_value);
                HAL_UART_Transmit(&huart2, (uint8_t *)response, strlen(response), 100);
                setTimer(1, 3000);
                state = 1;
            }
            break;

        case 1: // Chờ  !OK#
            if (command_flag == 2) { // Khi nhận được lệnh !OK#
                HAL_UART_Transmit(&huart2, end_data, sizeof(end_data), 100);
                state = 0; // Quay về trạng thái chờ lệnh !RST#
            } else  { // Nếu hết 3 giây mà không nhận được !OK#
            	update_handlemode();
            }
            break;

        default:
            state = 0;
    }

    command_flag = 0;
}
void run(){
    HAL_UART_Transmit(&huart2, (uint8_t *)response, strlen(response), 100); // Gửi lại giá trị ADC
}
