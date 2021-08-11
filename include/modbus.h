/*
 * modbus_app.h
 *
 *  Created on: 16 gru 2020
 *      Author: mkowalski
 */

#ifndef MODBUS_APP_H_
#define MODBUS_APP_H_

#include "mb.h"
#include "mbport.h"

/*-------------------- Defines --------------------*/

#define REG_INPUT_START 	1000
#define REG_INPUT_NREGS 	10

#define REG_HOLDING_START	2000
#define REG_HOLDING_NREGS	10

extern USHORT   usRegInputBuf[REG_INPUT_NREGS];
extern USHORT	usRegHoldingBuf[REG_HOLDING_NREGS];

void			MB_init(void);
eMBErrorCode	eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode	eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
eMBErrorCode	eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode );
eMBErrorCode 	eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );

extern uint32_t mb_baudrate;
extern uint32_t mb_ID;



#endif /* MODBUS_APP_H_ */
