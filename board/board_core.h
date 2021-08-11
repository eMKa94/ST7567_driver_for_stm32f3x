/*
    Board_core.h
    Board core Driver header file
*/

#ifndef BOARD_CORE_H
#define BOARD_CORE_H

#include <stdint.h>
#include "board.h"

extern uint64_t board_runtime_ms;
extern void board_1ms_routine(void);
extern void board_100ms_routine(void);
extern void board_1s_routine(void);

void board_core_init(void);
void board_wait_ms(uint32_t delay_ms);

void boardReboot(void);

void fpuEnable(void);

#endif