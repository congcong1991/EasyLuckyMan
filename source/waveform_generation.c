#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include <fsl_tpm.h>
#include "clock_config.h"
#include "MKL16Z4.h"
/* TODO: insert other include files here. */
#include "conf.h"
#include "waveform_generation.h"
#include "fsl_dac.h"
#include "fsl_pit.h"
#include "fsl_dmamux.h"
#include "fsl_dma.h"

//地址对齐
int x __attribute__ ((aligned (64)));

uint16_t Sin_Wave[32] __attribute__ ((aligned(64))) = {
		2047,2447,2831,3185,3495,3750,3939,4056,4095,4056,3939,3750,3495,3185,2831,2447,2047,1647,1263,909,599,344,155,38,0,38,155,344,599,909,1263,1647,
};


void DMA0_IRQHandler()
{
	TPM_StopTimer(TPM0);
    TPM0->SC |= 0x80;  //clear tpm0 overflow flag
	TPM0->CNT = 0x0;
	DMA_ClearChannelStatusFlags(DMA0,0,DMA_DSR_BCR_DONE_MASK); //clear dma0 ch0 done flag
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(640000);
	TPM_StartTimer(TPM0, kTPM_SystemClock);
	GPIO_TogglePinsOutput(GPIOB,1);
}

void initTPM0(void){

	tpm_config_t config;
	CLOCK_SetTpmClock(1);
	TPM_GetDefaultConfig(&config);
	config.prescale = kTPM_Prescale_Divide_1;  /* 24MHz */
	config.useGlobalTimeBase = false;
	config.enableDoze = true;                 /* TPM counter continues in doze mode */
	config.enableDebugMode = false;
	config.enableReloadOnTrigger = false;
	config.enableStopOnOverflow = false;
	config.enableStartOnTrigger = false;
	config.triggerSelect = kTPM_Trigger_Select_4;
	TPM_Init(TPM0, &config);
		TPM0->SC |= TPM_SC_DMA_MASK ; /* open DMA */
	TPM_SetTimerPeriod(TPM0, 74);  /* 320KHz */
	TPM0->CNT = 0x0;
	//TPM_EnableInterrupts(TPM0,  kTPM_TimeOverflowInterruptEnable);
	//NVIC_SetPriority(TPM0_IRQn, 2);
	//NVIC_EnableIRQ(TPM0_IRQn);
	TPM_StartTimer(TPM0, kTPM_SystemClock);
	GPIO_TogglePinsOutput(GPIOB,1);
}
void TPM0_IRQHandler(void){
//	TPM0->SC |= 0x80;
	GPIO_TogglePinsOutput(GPIOB,1);
}

void init_DMA0()
{
	dma_transfer_config_t dma0_transfer_config;
	CLOCK_EnableClock(kCLOCK_Dmamux0);
	CLOCK_EnableClock(kCLOCK_Dma0);



	dma0_transfer_config.destAddr = (uint32_t)&(DAC0->DAT[0].DATL);
	dma0_transfer_config.destSize = kDMA_Transfersize16bits;
	dma0_transfer_config.enableDestIncrement = false;
	dma0_transfer_config.enableSrcIncrement = true;
	dma0_transfer_config.srcAddr = (uint32_t)Sin_Wave;
	dma0_transfer_config.srcSize = kDMA_Transfersize16bits;
	dma0_transfer_config.transferSize = 640000;

	DMA_SetTransferConfig(DMA0,0,&dma0_transfer_config);
	DMA_SetModulo(DMA0,0,kDMA_Modulo64Bytes,kDMA_ModuloDisable);
	DMA_EnableCycleSteal(DMA0,0,true);
	DMA_EnableChannelRequest(DMA0,0);
	DMA_EnableInterrupts(DMA0,0);

	DMAMUX_SetSource(DMAMUX0,0,54);   //set tpm0 overflow as dma request
	DMAMUX_EnableChannel(DMAMUX0,0);
	NVIC_SetPriority(DMA0_IRQn, 0);
	NVIC_EnableIRQ(DMA0_IRQn);
}



void init_waveform_generate()
{

	CLOCK_EnableClock(kCLOCK_PortE);
	CLOCK_EnableClock(kCLOCK_Dac0);
	PORT_SetPinMux(PORTE, 30, kPORT_PinDisabledOrAnalog);



	init_DMA0();
	initTPM0();

	dac_config_t dac0_config;
	dac0_config.enableLowPowerMode = false;
	dac0_config.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;   //select VDDA as DAC reference.

	DAC_Init(DAC0,&dac0_config);

	DAC_Enable(DAC0,true);       //enable dac0


}

