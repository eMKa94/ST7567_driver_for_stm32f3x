/*
    board_buttons.c
    IPA board buttons source file
*/

#include "board_buttons.h"


void escButtonHardwareInit(void);
void dnButtonHardwareInit(void);
void upButtonHardwareInit(void);
void okButtonHardwareInit(void);

button_t ESC_BUTTON;
button_t DN_BUTTON;
button_t UP_BUTTON;
button_t OK_BUTTON;

void boardButtonsInit(void)
{
	ESC_BUTTON = create_button();
	DN_BUTTON = create_button();
	UP_BUTTON = create_button();
	OK_BUTTON = create_button();

	escButtonHardwareInit();
	dnButtonHardwareInit();
	upButtonHardwareInit();
	okButtonHardwareInit();

	add_button(&ESC_BUTTON);
	add_button(&DN_BUTTON);
	add_button(&UP_BUTTON);
	add_button(&OK_BUTTON);

}

void escButtonHardwareInit(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(ESC_BUTTON_GPIO_PORT, ESC_BUTTON_GPIO_PIN, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(ESC_BUTTON_GPIO_PORT, ESC_BUTTON_GPIO_PIN, LL_GPIO_PULL_UP);
}

void dnButtonHardwareInit(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(DN_BUTTON_GPIO_PORT, DN_BUTTON_GPIO_PIN, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(DN_BUTTON_GPIO_PORT, DN_BUTTON_GPIO_PIN, LL_GPIO_PULL_UP);
}

void upButtonHardwareInit(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(UP_BUTTON_GPIO_PORT, UP_BUTTON_GPIO_PIN, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(UP_BUTTON_GPIO_PORT, UP_BUTTON_GPIO_PIN, LL_GPIO_PULL_UP);
}

void okButtonHardwareInit(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(OK_BUTTON_GPIO_PORT, OK_BUTTON_GPIO_PIN, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(OK_BUTTON_GPIO_PORT, OK_BUTTON_GPIO_PIN, LL_GPIO_PULL_UP);
}

void buttons_state_update(void)
{
	ESC_BUTTON.temp_state	= (readEscButton()?	pressed : released);
	DN_BUTTON.temp_state	= (readDnButton()?	pressed : released);
	UP_BUTTON.temp_state	= (readUpButton()?	pressed : released);
	OK_BUTTON.temp_state	= (readOkButton()?	pressed : released);
}