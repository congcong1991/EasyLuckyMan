/*
 * kl16z64.c
 *
 *  Created on: Nov 21, 2017
 *      Author: zzh
 */
#include "conf.h"
#include <fsl_uart.h>
#include <fsl_gpio.h>
#include <fsl_port.h>

#define UART_SRC_CLOCK 24000000
uint8_t ms1=0;  // 1ms标志
void setCom(uint8_t baudrate,uint8_t parity)
{
		uint32_t br;
		DisableIRQ(UART1_IRQn);
		UART_DisableInterrupts(UART1, kUART_RxDataRegFullInterruptEnable);
		UART_DisableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable);

		/*-------------------------USART参数设定------------------------*/
		if(baudrate==0x0)br=600;		    //  600
		else if(baudrate==0x1)br=1200;	// 1200
		else if(baudrate==0x2)br=2400;	// 2400
		else if(baudrate==0x3)br=4800;	// 4800
		else if(baudrate==0x4)br=9600;	//9600
		else if(baudrate==0x5)br=19200;  // 19200
		else if(baudrate==0x6)br=38400; // 38400
		else if(baudrate==0x7)br=57600;  // 57600
		else if(baudrate==0x8)br=115200; // 115200
		else br=9600;

		{
			uart_config_t uart_config;

			UART_Deinit(UART1);
			UART_GetDefaultConfig(&uart_config);
			uart_config.baudRate_Bps=br;			/* 数据波特率　*/
			uart_config.stopBitCount=1;  /* 1位停止位  */

			if(parity==0){uart_config.parityMode=kUART_ParityDisabled;  /* 无校验  */}
			if(parity==1){uart_config.parityMode=kUART_ParityOdd; }// 奇校验
			if(parity==2){uart_config.parityMode=kUART_ParityEven;}// 偶校验



			uart_config.enableTx=1;
			uart_config.enableRx=1;
			uart_config.stopBitCount=kUART_OneStopBit;

			UART_Init(UART1, &uart_config, UART_SRC_CLOCK);  // 24000000
		}

		UART_ClearStatusFlags(UART1,kUART_TransmissionCompleteFlag);
		UART_EnableInterrupts(UART1, kUART_RxDataRegFullInterruptEnable);
		UART_EnableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable);
//		USART_Cmd(USART1, ENABLE);//使能串口1
		NVIC_SetPriority(UART1_IRQn, 2);
		EnableIRQ(UART1_IRQn);
}
void setBaudrate(uint8_t baudrate)
{
	setCom(baudrate,config.parity);
}
void setParity(uint8_t parity)
{
	setCom(config.baundrate,parity);
}
void Enable485TXD(void) {
	GPIO_SetPinsOutput(GPIOC,1<<2);
	UART_ClearStatusFlags(UART1,kUART_TransmissionCompleteFlag);
	UART_EnableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable);

}
void Disable485TXD(void)
{
	GPIO_ClearPinsOutput(GPIOC,1<<2);
	UART_DisableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable);
}


void initModbus()
{

	NVIC_SetPriority (UART1_IRQn, 1);  // 串口中断优先级

	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_Uart1);

	/*------------------配置USART端口-----------------------------*/

	PORT_SetPinMux(PORTC, 3, kPORT_MuxAlt3);  //TXD
	PORT_SetPinMux(PORTC, 4, kPORT_MuxAlt3);  //RXD
	PORT_SetPinMux(PORTC, 2, kPORT_MuxAsGpio);  //RS

	{//PC2 RS  1:TXD
		port_pin_config_t pin_config;
		pin_config.driveStrength=1;
		pin_config.mux=1;
		pin_config.passiveFilterEnable=0;
		pin_config.pullSelect=0;
		pin_config.slewRate=0;
		PORT_SetPinConfig(PORTC, 2, &pin_config);

		gpio_pin_config_t gpio_config;
		gpio_config.pinDirection=kGPIO_DigitalOutput;
		gpio_config.outputLogic=0;
		GPIO_PinInit(GPIOC, 2, &gpio_config);
	}
	// 关闭串口
	Disable485TXD();
	/*-------------------------USART参数设定------------------------*/
	setCom(config.baundrate,config.parity);

}
unsigned char CheckEven(unsigned char becheck)
{
		return(0);
}
void startTxd()
{

#ifndef DISABLE_MODBUS
	Enable485TXD();

//	UART_WriteByte(UART1,getTxdByte());
#endif
}

void UART1_IRQHandler(void)
{
	uint8_t c;
	if(UART_GetStatusFlags(UART1)&kUART_RxDataRegFullFlag )  //接收中断
	{
#ifndef DISABLE_MODBUS
			RxdByte((uint8_t)(UART_ReadByte(UART1)));		   // 收到一个字节
#endif
			UART_ClearStatusFlags(UART1, kUART_RxDataRegFullFlag);
	}
	else if(kUART_TransmissionCompleteFlag&UART_GetStatusFlags(UART1))   // 发送缓冲空
	{

#ifndef DISABLE_MODBUS
		if(hasByteToTxd())		{
			Enable485TXD();
			c=getTxdByte();
			UART_WriteByte(UART1,c);
			UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag);

		}
		else	{
			Disable485TXD();
			UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag);

		}
#endif
	}	else 	{
//		USART1->ICR=0xffffffff;
	}

}

//-----------------------------------------------------|
//定时器0中断子程序	  1ms 一次中断 ，用于系统计时和调度
//-----------------------------------------------------|
//************************************************
//***********************************************/
extern void On1ms(void);   // main.c中定义
void initSysTick1ms()
{
	NVIC_SetPriority (SysTick_IRQn,3);  //中断优先级
	EnableIRQ(SysTick_IRQn);


//		TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);  //打开中断
	if (SysTick_Config(SystemCoreClock / 1000))//1毫秒
	{
			/* Capture error */
			while (1);
		}
}
//void Init_Time16(uint16_t num)    //  num 定时时长，  1uS 的倍数
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

//	TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
//	TIM_DeInit(TIM16);
//
//	  /*  TIM16 中断优先级 */
//  TIM_TimeBaseStructure.TIM_Period = num;       //定时器计数值
//  TIM_TimeBaseStructure.TIM_Prescaler =47;    //时钟预分频值(1uS)
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数模式(向上计数)
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//  TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
//
//	TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
//	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM16,ENABLE);    // 开启定时器
//}
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
	ms1=1; // 1ms到达
}


//void TIM16_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM16, TIM_IT_Update); //清除中断等待标志位
//		TIM_Cmd(TIM16,DISABLE);

//	}
//}


