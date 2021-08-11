#ifndef GUI_H
#define GUI_H

#include "gui_core.h"
#include "board.h"

void guiInit(void);

// Voltage L-N display menu
extern guiObject_t lnVoltageL1Object;
extern guiObject_t lnVoltageL2Object;
extern guiObject_t lnVoltageL3Object;
void enterLnVoltageMenu(void);
void renderLnVoltageMenu(void);

// Settings navigation menu

extern guiObject_t dateTimeSettingsObject;
extern guiObject_t modbusSettingsObject;
extern guiObject_t displaySettingsObject;
extern guiObject_t InputOutputSettingsObject;
extern guiObject_t calibrationSettingsObject;
extern guiObject_t deviceInfoSettingsObject;
void enterSettingsMenu(void);
void renderSettingsMenu(void);

// Date and Time Settings Menu
extern guiObject_t daySettingsObject;
extern guiObject_t monthSettingsObject;
extern guiObject_t yearSettingsObject;
extern guiObject_t hourSettingsObject;
extern guiObject_t minuteSettingsObject;
void enterDateTimeSettingsMenu(void);
void renderDateTimeSettingsMenu(void);

// Modbus Settings Menu
extern guiObject_t modbusIdObject;
extern guiObject_t rs485BaudrateObject;
extern guiObject_t rs485ParityObject;
extern guiObject_t rs485StopBitsObject;
void enterModbusSettingsMenu(void);
void renderModbusSettingsMenu(void);

// Display Settings Menu
extern guiObject_t backlightOnTimeObject;
extern guiObject_t backlightOnLevelObject;
extern guiObject_t backlightOffLevelObject;
void enterDisplaySettingsMenu(void);
void renderDisplaySettingsMenu(void);

// Device info menu
void enterDeviceInfoMenu(void);
void renderDeviceInfoMenu(void);

extern guiObject_t BackLightOnTimeMenuObject;

extern uint8_t guiDisplayBuffer[BOARD_LCD_BUFFER_SIZE];

void setEscButtonClickFunction(void(*functionPointer)(void));
void setDnButtonClickFunction(void(*functionPointer)(void));
void setUpButtonClickFunction(void(*functionPointer)(void));
void setOkButtonClickFunction(void(*functionPointer)(void));

void guiEscButtonClicked(void);
void guiDnButtonClicked(void);
void guiUpButtonClicked(void);
void guiOkButtonClicked(void);

void settingsSelectionMarkerUp(void);
void settingsSelectionMarkerDown(void);

void selectedObjectValueUp(void);
void selectedObjectValueDown(void);
void selectedObjectValueSave(void);

void editSelectedObject(void);

void enterObject(void);

void blOnTimeUp(void);
void blOnTimeDn(void);
void blOnTimeSave(void);

#endif