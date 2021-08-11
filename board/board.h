/*
#	#	#	BOARD   H	#	#	#
#								#
#		eMKa elektronika		#
#		Maciej Kowalski			#
#								#
#	STM32F373RCT6 prototype		#
#		board header file		#	
#								#	
#	#	#	#	#	#	#	#	#
*/

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>

#include "board_core.h"
#include "board_led.h"
#include "board_lcd.h"
#include "board_buttons.h"
#include "board_rs485.h"

extern uint32_t boardLcdBacklightStandardOnTime;

extern uint8_t          modbusId;
extern rs485Baudrate_t  rs485Baudrate;
extern rs485Parity_t    rs485Parity;
extern rs485StopBits_t  rs485StopBits;


void boardLCDBacklightTurnOnStandardPeriod(void);

void modbusBaseTimerInit(uint16_t timer50usTicks);

#endif