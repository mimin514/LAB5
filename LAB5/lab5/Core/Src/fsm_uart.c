/*
 * fsm_uart.c
 *
 *  Created on: Dec 6, 2024
 *      Author: User
 */

#include "main.h"
#include "fsm_uart.h"
#include "global.h"
#include <stdio.h>
#include <string.h>

void uart_communiation_fsm() {
    static uint8_t state = 0;
    static uint32_t last_time = 0; // Lưu thời gian chờ !OK#
    uint32_t current_time = HAL_GetTick();

    switch (state) {
        case 0: // Chờ lệnh !RST#
            if (command_flag == 1) { // Khi nhận được lệnh !RST#
                HAL_ADC_Start(&hadc1); // Bắt đầu chuyển đổi ADC
                if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
                    ADC_value = HAL_ADC_GetValue(&hadc1); // Lấy giá trị ADC
                }
                HAL_ADC_Stop(&hadc1); // Dừng ADC

                // Gửi giá trị ADC qua UART
                sprintf(response, "!ADC=%ld#\r\n", ADC_value);
                HAL_UART_Transmit(&huart2, (uint8_t *)response, strlen(response), 100);

                // Chuyển sang trạng thái chờ !OK#
                state = 1;
                last_time = current_time; // Lưu thời gian hiện tại
            }
            break;

        case 1: // Chờ lệnh !OK#
            if (command_flag == 2) { // Khi nhận được lệnh !OK#
                HAL_UART_Transmit(&huart2, end_data, sizeof(end_data), 100);
                state = 0; // Quay về trạng thái chờ lệnh !RST#
            } else if (current_time - last_time > 3000) { // Nếu hết 3 giây mà không nhận được !OK#
                HAL_UART_Transmit(&huart2, (uint8_t *)response, strlen(response), 100); // Gửi lại giá trị ADC
                last_time = current_time; // Cập nhật thời gian chờ
            }
            break;

        default:
            state = 0;
    }

    // Reset cờ sau khi xử lý
    command_flag = 0;
}
