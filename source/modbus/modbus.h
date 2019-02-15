#ifndef _MODBUS_H
#define _MODBUS_H
#include "conf.h"

#define  RX_BUFFER_SIZE 0x40
#define  TX_BUFFER_SIZE 0x60
extern uint8_t 		 TxdBuf[];
extern uint8_t       RxdBuf[];
extern uint8_t 	    RxdTelBuf[];
#define getRxdTelCommand() (RxdTelBuf[1])
extern void initModbus(void);

extern void RxdByte(uint8_t c);
extern uint8_t hasByteToTxd(void);		 // =1  有节字要发送
extern uint8_t getTxdByte(void);
extern uint8_t CheckEven(uint8_t c);
extern void startTxd(void);   //

extern void modbusLoop(void);

extern uint16_t CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen);
extern uint16_t CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen);


extern const 	uint8_t      auchCRCHi[];
extern const 	 uint8_t   	  auchCRCLo[];

void MakeComUpdata(void);
void MakeComTxdFrame(void);

#endif										
