#include "gui.h"
#include <string.h>

uint8_t IpaGuiBuffer[BOARD_LCD_BUFFER_SIZE];

uint8_t navigationIndexTop = 0;

guiObject_t *(*guiCurrentObjectsList);

float sampleValue1 = 123.456;
int16_t sampleValue2 = 100;

bool settingsObjectEdit = false;

uint32_t sampleList[5]=
{
    100,
    200,
    1000,
    2000,
    3000
};

extern float volatageRmsL1;
extern float volatageRmsL2;
extern float volatageRmsL3;

void (*EscButtonClickFunction)(void)    = &blankFunction;   
void (*DnButtonClickFunction)(void)     = &blankFunction;
void (*UpButtonClickFunction)(void)     = &blankFunction;
void (*OkButtonClickFunction)(void)     = &blankFunction;

void guiInit(void)
{
    setGuiDisplayBuffer(IpaGuiBuffer);
    board_lcd_set_display_data_address(IpaGuiBuffer);
    assignButtonPressFunction(&ESC_BUTTON, &guiEscButtonClicked);
    assignButtonPressFunction(&DN_BUTTON, &guiDnButtonClicked);
    assignButtonPressFunction(&UP_BUTTON, &guiUpButtonClicked);
    assignButtonPressFunction(&OK_BUTTON, &guiOkButtonClicked);
    enterLnVoltageMenu();
}

// Phase Voltage Menu Window
guiObject_t lnVoltageL1Object=
{
    .objectLabel = "L1 :",
    .objectSourceValuePointer = (void *)&volatageRmsL1,
    .objectValueType = FLOAT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t lnVoltageL2Object=
{
    .objectLabel = "L2 :",
    .objectSourceValuePointer = (void *)&volatageRmsL2,
    .objectValueType = FLOAT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t lnVoltageL3Object=
{
    .objectLabel = "L3 :",
    .objectSourceValuePointer = (void *)&volatageRmsL3,
    .objectValueType = FLOAT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

void enterLnVoltageMenu(void)
{
    setRenderFunction(&renderLnVoltageMenu);

    setEscButtonClickFunction(&blankFunction);
    setDnButtonClickFunction(&blankFunction);
    setUpButtonClickFunction(&blankFunction);
    setOkButtonClickFunction(&enterSettingsMenu);

    reloadObjectDisplayedValue(&lnVoltageL1Object);
    reloadObjectDisplayedValue(&lnVoltageL2Object);
    reloadObjectDisplayedValue(&lnVoltageL3Object);

}

void renderLnVoltageMenu(void)
{
    renderTextOnCenter("Napiecie L-N", 0);

    renderObjectLabel(&lnVoltageL1Object, 2, 30);
    renderObjectValue(&lnVoltageL1Object, 2, 60);

    renderObjectLabel(&lnVoltageL2Object, 4, 30);
    renderObjectValue(&lnVoltageL2Object, 4, 60);
    
    renderObjectLabel(&lnVoltageL3Object, 6, 30);
    renderObjectValue(&lnVoltageL3Object, 6, 60);
    
}

// Settings Navigation Menu Window

guiObject_t dateTimeSettingsObject =
{
    .objectLabel = "Data / Godzina",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterDateTimeSettingsMenu
};
guiObject_t modbusSettingsObject =
{
    .objectLabel = "Modbus RTU",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterModbusSettingsMenu
};
guiObject_t displaySettingsObject =
{
    .objectLabel = "Wyswietlacz",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterDisplaySettingsMenu
};
guiObject_t InputOutputSettingsObject =
{
    .objectLabel = "Wejscia / wyjscia",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterDateTimeSettingsMenu
};
guiObject_t calibrationSettingsObject =
{
    .objectLabel = "Kalibracja",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterDateTimeSettingsMenu
};

guiObject_t deviceInfoSettingsObject =
{
    .objectLabel = "Informacje",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterDateTimeSettingsMenu    
};

guiObject_t *settingMenuObjects[6]=
{
    &dateTimeSettingsObject,
    &modbusSettingsObject,
    &displaySettingsObject,
    &InputOutputSettingsObject,
    &calibrationSettingsObject,
    &deviceInfoSettingsObject
};

uint8_t settingsNavigationIndex = 0;
void settingsNavigationMarkerUp(void)
{
    if (settingsNavigationIndex > 0) settingsNavigationIndex--;
}

void settingsNavigationMarkerDown(void)
{
    if (settingsNavigationIndex < navigationIndexTop) settingsNavigationIndex++;
}

void enterSettingsMenu(void)
{
    setRenderFunction(&renderSettingsMenu);

    setEscButtonClickFunction(&enterLnVoltageMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);
    settingsNavigationIndex = 0;
    navigationIndexTop = 4;
    guiCurrentObjectsList = settingMenuObjects;
}

void renderSettingsMenu(void)
{
    
    renderTextOnCenter("USTAWIENIA", 0);

    renderObjectLabel(settingMenuObjects[0], 2, 10);
    renderObjectLabel(settingMenuObjects[1], 3, 10);
    renderObjectLabel(settingMenuObjects[2], 4, 10);
    renderObjectLabel(settingMenuObjects[3], 5, 10);
    renderObjectLabel(settingMenuObjects[4], 6, 10);

    uint8_t markerPage      = settingMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = settingMenuObjects[settingsNavigationIndex]->objectColumnPosition - 10;
    renderSelectionMarker(markerPage, markerColumn);

}

// Date and Time Settings Menu
guiObject_t daySettingsObject =
{
    .objectLabel = "Dzien   :",
    .objectDisplayedIntValue = 10,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t monthSettingsObject =
{
    .objectLabel = "Miesiac :",
    .objectDisplayedIntValue = 7,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t yearSettingsObject =
{
    .objectLabel = "Rok     :",
    .objectDisplayedIntValue = 2021,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t hourSettingsObject =
{
    .objectLabel = "Godzina :",
    .objectDisplayedIntValue = 11,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t minuteSettingsObject =
{
    .objectLabel = "Minuta  :",
    .objectDisplayedIntValue = 44,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t *dateTimeMenuObjects[5]=
{
    &daySettingsObject,
    &monthSettingsObject,
    &yearSettingsObject,
    &hourSettingsObject,
    &minuteSettingsObject
};
void enterDateTimeSettingsMenu(void)
{
    setRenderFunction(renderDateTimeSettingsMenu);

    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);
    settingsNavigationIndex = 0;
    navigationIndexTop = 4;

    guiCurrentObjectsList = dateTimeMenuObjects;
}

void renderDateTimeSettingsMenu(void)
{
    renderTextOnCenter("Ust. Daty i Godz.", 0);

    renderObjectLabel(dateTimeMenuObjects[0], 2, 10);
    renderObjectValue(dateTimeMenuObjects[0], 2, 80);

    renderObjectLabel(dateTimeMenuObjects[1], 3, 10);
    renderObjectValue(dateTimeMenuObjects[1], 3, 80);
    
    renderObjectLabel(dateTimeMenuObjects[2], 4, 10);
    renderObjectValue(dateTimeMenuObjects[2], 4, 80);

    renderObjectLabel(dateTimeMenuObjects[3], 5, 10);
    renderObjectValue(dateTimeMenuObjects[3], 5, 80);

    renderObjectLabel(dateTimeMenuObjects[4], 6, 10);
    renderObjectValue(dateTimeMenuObjects[4], 6, 80);

    uint8_t markerPage      = dateTimeMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = dateTimeMenuObjects[settingsNavigationIndex]->objectColumnPosition - 10;
    renderSelectionMarker(markerPage, markerColumn);

}

// Modbus Settings Menu
void reloadModbusSettings(void)
{
    modbusIdObject.objectDisplayedIntValue      = (int32_t)modbusId;
    rs485BaudrateObject.objectDisplayedIntValue = (int32_t)rs485Baudrate;
    rs485ParityObject.objectDisplayedIntValue   = (int32_t)rs485Parity;
    rs485StopBitsObject.objectDisplayedIntValue = (int32_t)rs485StopBits;    
}

void cancelModbusObjectEdit(void)
{
    settingsObjectEdit = false;
    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);

    reloadModbusSettings();
}

void editModbusObject(void)
{
    settingsObjectEdit = true;

    setEscButtonClickFunction(&cancelModbusObjectEdit);
    setDnButtonClickFunction(&selectedObjectValueDown);
    setUpButtonClickFunction(&selectedObjectValueUp);
    setOkButtonClickFunction(&selectedObjectValueSave);
}

extern void MB_init(void);
void mbIdIncrement(void)
{
    if (modbusIdObject.objectDisplayedIntValue < 100) modbusIdObject.objectDisplayedIntValue ++;
}
void mbIdDecrement(void)
{
    if (modbusIdObject.objectDisplayedIntValue > 1) modbusIdObject.objectDisplayedIntValue --;
}
void mbIdSave(void)
{
    modbusId = (uint8_t)modbusIdObject.objectDisplayedIntValue;
    MB_init();
    cancelModbusObjectEdit();
}

void mbBaudrateIncrement(void)
{
    if (rs485BaudrateObject.objectDisplayedIntValue < 5) rs485BaudrateObject.objectDisplayedIntValue ++; 
}
void mbBaudrateDecrement(void)
{
    if (rs485BaudrateObject.objectDisplayedIntValue > 0) rs485BaudrateObject.objectDisplayedIntValue --; 
}
void mbBaudratesave(void)
{
    rs485Baudrate = (rs485Baudrate_t)rs485BaudrateObject.objectDisplayedIntValue;
    MB_init();
    cancelModbusObjectEdit();
}

void rs485ParityIncrement(void)
{
    if (rs485ParityObject.objectDisplayedIntValue < 2) rs485ParityObject.objectDisplayedIntValue ++;
}
void rs485ParityDecrement(void)
{
    if (rs485ParityObject.objectDisplayedIntValue > 0) rs485ParityObject.objectDisplayedIntValue --;
}
void rs485Paritysave(void)
{
    rs485Parity = (rs485Parity_t)rs485ParityObject.objectDisplayedIntValue;
    MB_init();
    cancelModbusObjectEdit();
}

void rs485StopBitsIncrement(void)
{
    if (rs485StopBitsObject.objectDisplayedIntValue < 1) rs485StopBitsObject.objectDisplayedIntValue ++;
}
void rs485StopBitsDecrement(void)
{
    if (rs485StopBitsObject.objectDisplayedIntValue  > 0) rs485StopBitsObject.objectDisplayedIntValue --;
}
void rs485StopBitssave(void)
{
    rs485StopBits = (rs485StopBits_t)rs485StopBitsObject.objectDisplayedIntValue;
    MB_init();
    cancelModbusObjectEdit();
}

guiObject_t modbusIdObject =
{
    .objectLabel = "ID       :",
    .objectDisplayedIntValue = 1,
    .objectValueType = INT,
    .valueDnFunction = &mbIdDecrement,
    .valueUpFunction = &mbIdIncrement,
    .valueSaveFunction = &mbIdSave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t rs485BaudrateObject =
{
    .objectLabel = "Badurate :",
    .objectDisplayedIntValue = 0,
    .objectValueType = INT,
    .valueDnFunction = &mbBaudrateDecrement,
    .valueUpFunction = &mbBaudrateIncrement,
    .valueSaveFunction = &mbBaudratesave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t rs485ParityObject =
{
    .objectLabel = "Parzyst. :",
    .objectDisplayedIntValue = 0,
    .objectValueType = INT,
    .valueDnFunction = &rs485ParityDecrement,
    .valueUpFunction = &rs485ParityIncrement,
    .valueSaveFunction = &rs485Paritysave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t rs485StopBitsObject =
{
    .objectLabel = "Stop bit :",
    .objectDisplayedIntValue = 1,
    .objectValueType = INT,
    .valueDnFunction = &rs485StopBitsDecrement,
    .valueUpFunction = &rs485StopBitsIncrement,
    .valueSaveFunction = &rs485StopBitssave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t *modbusMenuObjects[4]=
{
    &modbusIdObject,
    &rs485BaudrateObject,
    &rs485ParityObject,
    &rs485StopBitsObject    
};


void enterModbusSettingsMenu(void)
{
    setRenderFunction(&renderModbusSettingsMenu);

    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);
    settingsNavigationIndex = 0;
    settingsObjectEdit = false;
    navigationIndexTop = 3;

    reloadModbusSettings();

    guiCurrentObjectsList = modbusMenuObjects;
}


void renderModbusSettingsMenu(void)
{
    renderTextOnCenter("Ust. Modbus / rs485", 0);

    renderObjectLabel(&modbusIdObject, 2, 10);
    renderObjectValue(&modbusIdObject, 2, 80);

    renderObjectLabel(&rs485BaudrateObject, 3, 10);
    // renderObjectValue(modbusMenuObjects[1], 3, 80);
    switch (rs485BaudrateObject.objectDisplayedIntValue)
    {
        case baudrate_4800:
            renderInt(4800, 3, 80);
            break;
        
        case baudrate_9600:
            renderInt(9600, 3, 80);
            break;
        
        case baudrate_19200:
            renderInt(19200, 3, 80);
            break;
        
        case baudrate_38400:
            renderInt(38400, 3, 80);
            break;
        
        case baudrate_57600:
            renderInt(57600, 3, 80);
            break;
        
        case baudrate_115200:
            renderInt(115200, 3, 80);
            break;

        default:
            break;

    }

    renderObjectLabel(&rs485ParityObject, 4, 10);
    switch(rs485ParityObject.objectDisplayedIntValue)
    {
        case parity_none:
            renderText("---", 4, 80);
            break;
        case parity_even:
            renderText("par", 4, 80);
            break;
        case parity_odd:
            renderText("n-par", 4, 80);
            break;
        default:
            break;
    }

    renderObjectLabel(&rs485StopBitsObject, 5, 10);
    renderInt(rs485StopBitsObject.objectDisplayedIntValue+1, 5, 80);

    uint8_t markerPage      = modbusMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = modbusMenuObjects[settingsNavigationIndex]->objectColumnPosition;
    renderSelectionMarker(markerPage, markerColumn + (settingsObjectEdit ? 60 : -10));

}

// Display Settings Menu
guiObject_t backlightOnTimeObject =
{
    .objectLabel = "Czas wl.     :",
    .objectSourceValuePointer = (void *)&boardLcdBacklightStandardOnTime,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t backlightOnLevelObject =
{
    .objectLabel = "Jasnosc wl.  :",
    //.objectSourceValuePointer = (void *)&boardLcdBacklightStandardOnTime,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t backlightOffLevelObject =
{
    .objectLabel = "Jasnosc wyl. :",
    //.objectSourceValuePointer = (void *)&boardLcdBacklightStandardOnTime,
    .objectValueType = INT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t *displayMenuObjects[3] =
{
    &backlightOnTimeObject,
    &backlightOnLevelObject,
    &backlightOffLevelObject
};

void enterDisplaySettingsMenu(void)
{
    setRenderFunction(&renderDisplaySettingsMenu);

    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);
    settingsNavigationIndex = 0;
    navigationIndexTop = 2;

    guiCurrentObjectsList = displayMenuObjects;

}

void renderDisplaySettingsMenu(void)
{
    
    renderTextOnCenter("Ust. wyswietlacza", 0);

    renderObjectLabel(displayMenuObjects[0], 2, 10);
    renderObjectValue(displayMenuObjects[0], 2, 95);

    renderObjectLabel(displayMenuObjects[1], 3, 10);
    renderObjectValue(displayMenuObjects[1], 3, 95);
    
    renderObjectLabel(displayMenuObjects[2], 4, 10);
    renderObjectValue(displayMenuObjects[2], 4, 95);

    uint8_t markerPage      = displayMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = displayMenuObjects[settingsNavigationIndex]->objectColumnPosition - 10;
    renderSelectionMarker(markerPage, markerColumn);

}

guiObject_t sampleObject1=
{
    .objectLabel = "Object 1 :",
    .objectSourceValuePointer = (void *)&sampleValue1,
    .objectDisplayedIntValue = 5,
    .objectValueType = FLOAT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &editSelectedObject
};

void so2change(void);
guiObject_t sampleObject2=
{
    .objectLabel = "Object 2 :",
    //.objectSourceValuePointer = (void *)&sampleValue2,
    .objectDisplayedBoolValue = true,
    .objectValueType = BOOLEAN,
    .valueDnFunction = &so2change,
    .valueUpFunction = &so2change,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &editSelectedObject
};

void so2change(void)
{
    sampleObject2.objectDisplayedBoolValue ^= true;
}

guiObject_t sampleObject3=
{
    .objectLabel = "Badurate :",
    .objectSourceValuePointer = (void *)&sampleList,
    .objectValueType = UINT_LIST,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &editSelectedObject
};

guiObject_t BackLightOnTimeMenuObject=
{
    .objectLabel = "BL T on  :",
    .objectSourceValuePointer = (void *)&boardLcdBacklightStandardOnTime,
    .objectValueType = INT,
    .valueDnFunction = &blOnTimeDn,
    .valueUpFunction = &blOnTimeUp,
    .valueSaveFunction = &blOnTimeSave,
    .objectEnterFunction = &editSelectedObject
};

void blOnTimeUp(void)
{
    if (BackLightOnTimeMenuObject.objectDisplayedIntValue < 100)
    {
        BackLightOnTimeMenuObject.objectDisplayedIntValue ++;
    }
}

void blOnTimeDn(void)
{
    if (BackLightOnTimeMenuObject.objectDisplayedIntValue > 1)
    {
        BackLightOnTimeMenuObject.objectDisplayedIntValue --;
    }
}

void blOnTimeSave(void)
{
    *(uint32_t *)(BackLightOnTimeMenuObject.objectSourceValuePointer) = BackLightOnTimeMenuObject.objectDisplayedIntValue;
    boardLCDBacklightTurnOnStandardPeriod();
    enterSettingsMenu();
}

void setEscButtonClickFunction(void(*functionPointer)(void))
{
    EscButtonClickFunction = functionPointer;
}

void setDnButtonClickFunction(void(*functionPointer)(void))
{
    DnButtonClickFunction = functionPointer;
}

void setUpButtonClickFunction(void(*functionPointer)(void))
{
    UpButtonClickFunction = functionPointer;
}

void setOkButtonClickFunction(void(*functionPointer)(void))
{
    OkButtonClickFunction = functionPointer;
}

void guiEscButtonClicked(void)
{
    EscButtonClickFunction();
}

void guiDnButtonClicked(void)
{
    DnButtonClickFunction();
}

void guiUpButtonClicked(void)
{
    UpButtonClickFunction();
}

void guiOkButtonClicked(void)
{
    OkButtonClickFunction();
}

void selectedObjectValueUp(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->valueUpFunction();
}

void selectedObjectValueDown(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->valueDnFunction();
}

void selectedObjectValueSave(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->valueSaveFunction();
}

void editSelectedObject(void)
{
    settingsObjectEdit = true;
    setEscButtonClickFunction(&enterSettingsMenu);
    setDnButtonClickFunction(&selectedObjectValueDown);
    setUpButtonClickFunction(&selectedObjectValueUp);
    setOkButtonClickFunction(&selectedObjectValueSave);
}

void enterObject(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->objectEnterFunction();
}