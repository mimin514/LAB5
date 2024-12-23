/*
 * fsm_cmd.c
 *
 *  Created on: Dec 6, 2024
 *      Author: User
 */


#include "fsm_cmd.h"
#include "global.h"
#include "main.h"

void command_parser_fsm() {
    static uint8_t state = 0;

    for (uint8_t i = 0; i < index_buffer; i++) {
        switch (state) {
            case 0:
                if (buffer[i] == '!') state = 1;
                break;
            case 1:
                if (buffer[i] == 'R') state = 2;
                else if (buffer[i] == 'O') state = 5;
                else state = 0;
                break;
            case 2:
                if (buffer[i] == 'S') state = 3;
                else state = 0;
                break;
            case 3:
                if (buffer[i] == 'T') state = 4;
                else state = 0;
                break;
            case 4:
                if (buffer[i] == '#') {
                    // Command received: !RST#
                    command_flag = 1;
                }
                state = 0;
                break;
            case 5:
                if (buffer[i] == 'K') state = 6;
                else state = 0;
                break;
            case 6:
                if (buffer[i] == '#') {
                    // Command received: !OK#
                    command_flag = 2;
                }
                state = 0;
                break;
            default:
                state = 0;
        }
    }
    index_buffer = 0; // Reset buffer after parsing
}
