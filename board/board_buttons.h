/*
    board_buttons.h
    IPA board buttons header file
*/

#include "buttons_driver.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"

extern button_t ESC_BUTTON;
extern button_t DN_BUTTON;
extern button_t UP_BUTTON;
extern button_t OK_BUTTON;


#ifndef BOARD_BUTTONS_H
#define BOARD_BUTTONS_H

#define ESC_BUTTON_GPIO_PORT	GPIOB
#define ESC_BUTTON_GPIO_PIN		LL_GPIO_PIN_9
#define readEscButton()			!(LL_GPIO_ReadInputPort(ESC_BUTTON_GPIO_PORT) & ESC_BUTTON_GPIO_PIN)

#define DN_BUTTON_GPIO_PORT		GPIOB
#define DN_BUTTON_GPIO_PIN		LL_GPIO_PIN_8
#define readDnButton()			!(LL_GPIO_ReadInputPort(DN_BUTTON_GPIO_PORT) & DN_BUTTON_GPIO_PIN)

#define UP_BUTTON_GPIO_PORT		GPIOB
#define UP_BUTTON_GPIO_PIN		LL_GPIO_PIN_7
#define readUpButton()			!(LL_GPIO_ReadInputPort(UP_BUTTON_GPIO_PORT) & UP_BUTTON_GPIO_PIN)

#define OK_BUTTON_GPIO_PORT		GPIOB
#define OK_BUTTON_GPIO_PIN		LL_GPIO_PIN_6
#define readOkButton()			!(LL_GPIO_ReadInputPort(OK_BUTTON_GPIO_PORT) & OK_BUTTON_GPIO_PIN)

void boardButtonsInit(void);

#endif