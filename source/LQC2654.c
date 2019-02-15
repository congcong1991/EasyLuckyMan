/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    YE5955V2.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL16Z4.h"
/* TODO: insert other include files here. */
#include "conf.h"
#include "waveform_generation.h"


/* TODO: insert other definitions and declarations here. */
void initRVStartPort(void){//PB1
	{
		port_pin_config_t pin_config;
		CLOCK_EnableClock(kCLOCK_PortB);
		pin_config.driveStrength=1;
		pin_config.mux=1;
		pin_config.passiveFilterEnable=0;
		pin_config.pullSelect=0;
		pin_config.slewRate=0;
		PORT_SetPinConfig(PORTB, 1, &pin_config);
	}
	{
		gpio_pin_config_t gpio_config;
		gpio_config.pinDirection=kGPIO_DigitalOutput;
		gpio_config.outputLogic=1;
		GPIO_PinInit(GPIOB, 1, &gpio_config);
	}
}
void initStartPWM(void){//PB0
	{
		port_pin_config_t pin_config;
		CLOCK_EnableClock(kCLOCK_PortB);
		pin_config.driveStrength=1;
		pin_config.mux=1;
		pin_config.passiveFilterEnable=0;
		pin_config.pullSelect=0;
		pin_config.slewRate=0;
		PORT_SetPinConfig(PORTB, 0, &pin_config);
	}
	{
		gpio_pin_config_t gpio_config;
		gpio_config.pinDirection=kGPIO_DigitalOutput;
		gpio_config.outputLogic=1;
		GPIO_PinInit(GPIOB, 0, &gpio_config);
	}
}

void  disableStartPWM(void){	GPIO_ClearPinsOutput(GPIOB,1<<0);}
void  enableStartPWM(void){	GPIO_SetPinsOutput(GPIOB,1<<0);}


static void inline loop2Mpu(void){	GPIO_ClearPinsOutput(GPIOE,1<<0);GPIO_ClearPinsOutput(GPIOE,1<<0);}
static void inline loop2Extern(void){	GPIO_ClearPinsOutput(GPIOE,1<<0);GPIO_SetPinsOutput(GPIOE,1<<0);}
static void inline loop2I(void){	GPIO_SetPinsOutput(GPIOE,1<<0);GPIO_ClearPinsOutput(GPIOE,1<<0);}
static void inline loop2V(void){	GPIO_SetPinsOutput(GPIOE,1<<0);GPIO_SetPinsOutput(GPIOE,1<<0);}


/*
 * @brief   Application entry point.
 */
extern uint8_t ms1;
extern void initSysTick1ms(void);
#define BEGIN_LOOPS 2000
#define END_LOOPS 22000
int main(void) {
	static uint32_t main_loops=0;
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    init_waveform_generate();
 //   BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
//	BOARD_InitDebugConsole();
    initRVStartPort();
    initStartPWM();

    initEEPROM();
    initModbus();

    initSysTick1ms();

    initEMU();


    /* Force the counter to be placed into memory. */

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
//    	modbusLoop();
    	if(ms1){ms1=0;  	}else{  		continue;   	}
    	if(main_loops==(BEGIN_LOOPS-1)){
 //   		enableStart();
 //   		loop2V();
      	}
    	if(main_loops>BEGIN_LOOPS){

    		if((main_loops%50)==0){  /* 50ms */
  //  			testTxd();
    			updateParameter();
    		}


    	}

    	if(main_loops<END_LOOPS)main_loops++;
    	else main_loops=BEGIN_LOOPS;
    }
    return 0 ;
}



