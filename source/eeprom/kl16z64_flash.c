#define FLASH_DRIVER_IS_FLASH_RESIDENT 0
#include "conf.h"
#include <fsl_flash.h>
#include <fsl_tpm.h>
/* KL16Z64 flash address 0x0000-0xffff */

#define EEPROM_START 0xfc00
#define EEPROM_LEN   0x400
flash_config_t flash_config;


void initEEPROM()
{

	loadConfig();

}
void EarsePage(uint32_t address)  //
{
	FLASH_Init(&flash_config);
	__disable_irq();
	FLASH_Erase(&flash_config, EEPROM_START, EEPROM_LEN, kFLASH_ApiEraseKey);
	__enable_irq();
}


void RDBYTES(uint32_t address,uint8_t num, uint8_t *Data)
{
	uint8_t count;
	for(count=0;count<num/2;count++)
	{
		*((uint16_t *)Data+count)=*((uint16_t *)address+count);
  }
}

void WRBYTES(uint32_t address,uint32_t num, uint8_t *Data)
{
	num=(num+3)&0xfffffffc;
	FLASH_Init(&flash_config);
	__disable_irq();
	FLASH_Program(&flash_config, address, (uint32_t*)Data, num);
	__enable_irq();
}		

void readProtect(void){
	uint32_t t=0xfffffBf4;
	uint32_t rt=*((uint32_t*)(unsigned char*)(0x040c));
	if((rt &0x0f )!=4)	WRBYTES(0x040C,4,(unsigned char*)&t);
}


void saveConfig()
{
	TPM_StopTimer(TPM0);
	TPM0->CNT = 0x0;
	EarsePage(EEPROM_START);
	WRBYTES(EEPROM_START,sizeof(struct CONFIG),(uint8_t*)&config);
	TPM_StartTimer(TPM0, kTPM_SystemClock);
}
void loadConfig()
{
	RDBYTES(EEPROM_START,sizeof(config.vaildsign),(unsigned char*)&config);
	if(config.vaildsign!=0x5CAB)
	{	
		config.vaildsign=0x5CAB;
		saveConfig();
		return;
	}
	RDBYTES(EEPROM_START,sizeof(struct CONFIG),((unsigned char*)&config));
}




