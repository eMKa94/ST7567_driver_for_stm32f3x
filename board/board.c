/*
	Zenex sp. z o. o.
	Maciej Kowalski

	IPA Board source file
*/

/* Includes ------------------------------------------------------------------*/

#include "board.h"
#include "stm32f3xx_ll_tim.h"

//Display
uint32_t boardLcdBacklightStandardOnTime = 300;

// Modbus
uint8_t          modbusId 		= 1;
rs485Baudrate_t  rs485Baudrate 	= baudrate_19200;
rs485Parity_t    rs485Parity 	= parity_none;
rs485StopBits_t  rs485StopBits 	= stopBits_1;

void boardLCDBacklightTurnOnStandardPeriod(void)
{
	board_lcd_backlight_enable(boardLcdBacklightStandardOnTime*1000);
}

