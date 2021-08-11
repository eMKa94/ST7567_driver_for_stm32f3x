/*
    Board_led.h
    Board LED Driver header file
*/

#ifndef BOARD_LED_H
#define BOARD_LED_H

#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_bus.h"


typedef enum board_led_state_t
{
    LED_OFF,
    LED_ON
}board_led_state_t;

ErrorStatus board_led_init(void);
void board_led_set_solid(board_led_state_t led_state);
void board_led_set_blink(uint32_t led_on_time_ms, uint32_t led_off_time_ms);
void board_led_routine(void);



#endif