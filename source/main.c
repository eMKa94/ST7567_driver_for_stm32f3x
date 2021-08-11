/*
  Zenex sp. z o. o.
  Maciej Kowalski

  IPA - Integrated Power Analyser
  Firmware main application

  No any licence yet =]

*/

#include "ipa_config.h"
#include "board.h"

#ifdef GUI_ENABLED
#include "gui.h"
#endif

#include "modbus.h"

#include <stdbool.h>

float volatageRmsL1 = 230.0;
float volatageRmsL2 = 230.1;
float volatageRmsL3 = 229.9;

void rs485Test(void)
{
  boardRs485SendChar((char)modbusId);
}

int main(void)
{
  
 	board_core_init();
  // /board_led_init();
  
  #ifdef GUI_ENABLED
	boardButtonsInit();
	board_lcd_init();
  boardLCDBacklightTurnOnStandardPeriod();
	assign_any_action_function(&boardLCDBacklightTurnOnStandardPeriod);
	assignButtonHoldFunction(&ESC_BUTTON, &boardReboot);
  guiInit();
  board_lcd_activate();
  #endif

  usRegInputBuf[0] = 0;
  usRegInputBuf[1] = 0;
  usRegInputBuf[2] = 0;
  usRegInputBuf[3] = 0;
  usRegInputBuf[4] = 0;
  usRegInputBuf[5] = 0;
  usRegInputBuf[6] = 0;
  usRegInputBuf[7] = 0;
  usRegInputBuf[8] = 0;
  usRegInputBuf[9] = 0;

  MB_init();


  while (1)
  {

    #ifdef GUI_ENABLED
    if ((board_runtime_ms % 50) == 0) renderGui();
    #endif
    eMBPoll(  );
  }

}

void board_1ms_routine(void)
{
  //board_led_routine();
  #ifdef GUI_ENABLED
	board_lcd_routine();
  #endif
	button_check_routine();
  
}

void board_100ms_routine(void)
{
  #ifdef GUI_ENABLED
  boardLcdRefresh();
  #endif
  button_action_routine();
}

void board_1s_routine(void)
{
    usRegInputBuf[0] = (uint16_t)board_runtime_ms;
    usRegInputBuf[1] += 1;
    usRegInputBuf[2] += 2;
    usRegInputBuf[3] += 3;
    usRegInputBuf[4] += 4;
    usRegInputBuf[5] += 5;
    usRegInputBuf[6] += 6;
    usRegInputBuf[7] += 7;
    usRegInputBuf[8] += 8;
    usRegInputBuf[9] += 9;
}

