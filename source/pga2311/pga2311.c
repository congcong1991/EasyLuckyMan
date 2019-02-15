/*
 * pga2311.c
 *
 *  Created on: Nov 21, 2017
 *      Author: zzh
 */
/* SPI1_SCK PD5 */
/* SPI1_CS  PD4 */
/* SPI1_MISO PD6 */
/* SPI1_MOSI PD7 */

#include <fsl_spi.h>
#include <fsl_port.h>

#define SPI_CLOCK 48000000

void initPGA2311(void){
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_Spi1);
	PORT_SetPinMux(PORTD, 4, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, 5, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, 6, kPORT_MuxAlt5);
	PORT_SetPinMux(PORTD, 7, kPORT_MuxAlt5);

	SPI_Deinit(SPI1);
	{
		spi_master_config_t spi_config;
		spi_config.baudRate_Bps=3000000;  /* 3M 频率 */
		spi_config.dataMode=kSPI_16BitMode;
		spi_config.direction=kSPI_MsbFirst;
		spi_config.enableMaster=1;
		spi_config.enableStopInWaitMode=0;
		spi_config.outputMode=kSPI_SlaveSelectAutomaticOutput;
		spi_config.phase=kSPI_SlaveSelectAutomaticOutput;
		spi_config.pinMode=kSPI_PinModeNormal;
		spi_config.polarity=kSPI_ClockPolarityActiveHigh;
		spi_config.rxWatermark=kSPI_RxFifoThreeFourthsFull;
		spi_config.txWatermark=kSPI_TxFifoOneFourthEmpty;

		SPI_MasterInit(SPI1, &spi_config,SPI_CLOCK);

		SPI_EnableFIFO(SPI1, 0);
	}
}
/* 高８位　右声道　*/
/* 低８位　左声道　*/
/* +31.5 dB to −95.5 dB with 0.5-dB Steps */
/* 0 -- -96.5dB ,  255 --  31.5dB */

uint16_t  setPGA2311(uint16_t g){
	if(SPI_GetStatusFlags(SPI1)&kSPI_TxBufferEmptyFlag){
		SPI_WriteData(SPI1, g);
	}
	if(SPI_GetStatusFlags(SPI1)&kSPI_RxBufferFullFlag){
		return(SPI_ReadData(SPI1));
	}else{
		return(0);
	}
}
