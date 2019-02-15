/*
 * kl16z64emu.c
 *
 *  Created on: Nov 24, 2017
 *      Author: zzh
 */

#include "conf.h"
#include <fsl_adc16.h>
#include <fsl_tpm.h>
#include "waveform_generation.h"

//通道号
#define X_CH 0x01
#define Y_CH 0x86
#define Z_CH 0x06
#define PT100_CH 0x87
#define RES_CH 0x05
#define CAP_CH 0x02
#define TAN_CH 0x0f
//通道配置
const adc16_channel_config_t X_chConfig={1,1,0};
const adc16_channel_config_t Y_chConfig={6,1,0};
const adc16_channel_config_t Z_chConfig={6,1,0};
const adc16_channel_config_t Pt100_chConfig={7,1,0};
const adc16_channel_config_t Res_chConfig={5,1,0};
const adc16_channel_config_t Cap_chConfig={2,1,0};
const adc16_channel_config_t Tan_chConfig={15,1,0};
//通道切换
static inline void toXch(void){ADC0->SC1[0]=ADC_SC1_ADCH(1)|ADC_SC1_AIEN_MASK;ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;}
static inline void toYch(void){ADC0->SC1[0]=ADC_SC1_ADCH(6)|ADC_SC1_AIEN_MASK;ADC0->CFG2 |= ADC_CFG2_MUXSEL_MASK;}
static inline void toZch(void){ADC0->SC1[0]=ADC_SC1_ADCH(6)|ADC_SC1_AIEN_MASK;ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;}

static inline void toPt100ch(void){ADC0->SC1[0]=ADC_SC1_ADCH(7)|ADC_SC1_AIEN_MASK;ADC0->CFG2 |= ADC_CFG2_MUXSEL_MASK;}
static inline void toRes_ch(void){ADC0->SC1[0]=ADC_SC1_ADCH(5)|ADC_SC1_AIEN_MASK;ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;}
static inline void toCap_ch(void){ADC0->SC1[0]=ADC_SC1_ADCH(2)|ADC_SC1_AIEN_MASK;ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;}
static inline void toTan_ch(void){ADC0->SC1[0]=ADC_SC1_ADCH(15)|ADC_SC1_AIEN_MASK;ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;}


uint16_t int_sqrt(uint32_t  x){
	register uint16_t last = 0;
	register uint16_t  res = 16;
	register uint8_t i;
//	x<<=4;  // 放大X 提高精度
	if (x == 0) return 0;
	for (i=0;i<64;i++){
		if((res>>1)==last)break;
//	while((res>>1)!=last){
		last = res>>1;
		res = (res + x / res)>>1;
	}
//	return ((res+2)>>2);
	return(res);
}
////////////
unsigned long long_sqrt(unsigned long x)
{
	register unsigned long op, res, one;
  op = x;
  res = 0;
 /* "one" starts at the highest power of four <= than the argument. */
	one = 1 << 30;  /* second-to-top bit set */
  while (one > op) one >>= 2;
  while (one != 0) {
		if (op >= res + one) {
			op -= res + one;
      res += one << 1;  // <-- faster than 2 * one
    }
		res >>= 1;
    one >>= 2;
	}
  return res;
}

void initTPM1(void){
	CLOCK_EnableClock(kCLOCK_Tpm1);
	//CLOCK_SetPllFllSelClock(1); //Selects the MCGPLLCLK  clock for various peripheral clocking options.
	CLOCK_SetTpmClock(1);
	/*input clock source is MCGPLL/2=24MHz  */
	        // Start Capture timer
	tpm_config_t config;
	TPM_GetDefaultConfig(&config);
	config.prescale = kTPM_Prescale_Divide_4;  /* 6MHz */
	config.useGlobalTimeBase = false;          /* Use internal TPM counter as timebase */
	config.enableDoze = true;                 /* TPM counter continues in doze mode */
	config.enableDebugMode = false;            /* TPM counter pauses when in debug mode */
	config.enableReloadOnTrigger = false;      /* TPM counter will not be reloaded on input trigger */
	config.enableStopOnOverflow = false;       /* TPM counter continues running after overflow */
	config.enableStartOnTrigger = false;        /* TPM counter starts on first trigger */
//	config.enablePauseOnTrigger = false;       /* TPM does not pause on trigger */
	config.triggerSelect = kTPM_Trigger_Select_2;        /* Choose trigger select 0 as input trigger for controlling counter operation */
//	config.triggerSource = kTPM_TriggerSource_External;   /* Choose external trigger source to control counter operation */
	TPM_Init(TPM1, &config);
//	TPM1->SC |= TPM_SC_DMA_MASK ; /* open DMA */
	TPM_SetTimerPeriod(TPM1, 74);  /* 80KHz */
	TPM1->CNT = 0x0;

	//TPM_EnableInterrupts(TPM1,  kTPM_TimeOverflowInterruptEnable);
	//NVIC_SetPriority(TPM1_IRQn, 2);
	//NVIC_EnableIRQ(TPM1_IRQn);

	TPM_StartTimer(TPM1, kTPM_SystemClock);

}
void TPM1_IRQHandler(void){
	TPM1->SC |= 0x80;
	GPIO_TogglePinsOutput(GPIOB,1);
}

void initEMU(void){			// 采样率

	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTC, 1, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTD, 5, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTD, 6, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTE, 16, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTE, 17, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTE, 18, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTE, 19, kPORT_PinDisabledOrAnalog);

	CLOCK_EnableClock(kCLOCK_Adc0);
	ADC16_Deinit(ADC0);
	{
		adc16_config_t adc16_config;
//		ADC16_GetDefaultConfig(&adc16_config);
	    adc16_config.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	    adc16_config.clockSource = kADC16_ClockSourceAlt0;  /* bus clock 24MHz */
	    adc16_config.enableAsynchronousClock = false;
	    adc16_config.clockDivider = kADC16_ClockDivider8;
	    adc16_config.resolution = kADC16_ResolutionSE16Bit;
	    adc16_config.longSampleMode = kADC16_LongSampleDisabled;
	    adc16_config.enableHighSpeed = false;
	    adc16_config.enableLowPower = false;
	    adc16_config.enableContinuousConversion = false;

		ADC16_Init(ADC0,&adc16_config);
		ADC16_DoAutoCalibration(ADC0);

	}
	if(1)
	{  /* set trigger */
		SIM->SOPT7 &= ~SIM_SOPT7_ADC0TRGSEL_MASK; 	/* TPM1 overflow as trig */
		SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(9);
		SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN(1);  /* Enable alt trig */
		SIM->SOPT7 &= ~SIM_SOPT7_ADC0PRETRGSEL_MASK;
		SIM->SOPT7 |= SIM_SOPT7_ADC0PRETRGSEL(0); /* pre_trig a */
	}

	ADC16_EnableHardwareTrigger(ADC0, true);
	NVIC_SetPriority(ADC0_IRQn, 1);
	EnableIRQ(ADC0_IRQn);


	toXch();


	initTPM1();  /* 设置定时器 */
}

void startEMU(void){

}

void stopEMU(void){

}

#define ACC_TAO  4096L
#define ACC_TAO_BITS 12
#define V_TAO  4096L
#define V_TAO_BITS 12
#define PRE_TAO  8192L
#define PRE_TAO_BITS 13
#define TEMP_TAO 4096L
#define TEMP_TAO_BITS 12
#define RES_TAO 4096L
#define RES_TAO_BITS 12
#define CAP_TAO 8192L
#define CAP_TAO_BITS 13
#define TAN_TAO 8192L
#define TAN_TAO_BITS 13
#define ACC_ZERO_TAO 4096L
#define ACC_ZERO_TAO_BITS 12
#define V_ZERO_TAO 512L
#define V_ZERO_TAO_BITS 9
#define PRE_ZERO_TAO 16384L
#define PRE_ZERO_TAO_BITS 14

#define ONES 8192L
#define ONES_BITS 13

struct EMU emu={
		0,0,32768L,32768L*ACC_ZERO_TAO,0,0,
		0,0,32768L,32768L*ACC_ZERO_TAO,0,0,
		0,0,32768L,32768L*ACC_ZERO_TAO,0,0,

		0,0,0,0,0,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,

};

uint32_t sample_count=0;
int32_t adc_re=0;
volatile int32_t vx;
volatile int32_t vx0;
volatile int32_t ax;
volatile int32_t ax0;
uint8_t LowSpeed_AD_Channel = 0;

void ADC0_IRQHandler(void)
{
	uint32_t ch=(ADC0->SC1[0] & ADC_SC1_ADCH_MASK);
    //GPIOB->PSOR = 1; //GPIO_SetPinsOutput(GPIOB,1<<0);
    if(ADC0->CFG2 & ADC_CFG2_MUXSEL_MASK)ch|=0x80;
    adc_re=ADC0->R[0];

	switch(ch){  // 当前通道
		case X_CH:
			toYch();

			emu.x_zero_last=adc_re+((uint64_t)emu.x_zero_last*(ACC_ZERO_TAO-1)>>ACC_ZERO_TAO_BITS); /* 计算直流分量 */
			emu.x_zero=emu.x_zero_last>>ACC_ZERO_TAO_BITS;
//			ax0=emu.x_zero;
//			ax=adc_re;
			adc_re-=emu.x_zero;				/* 消零 */
			adc_re=adc_re*config.scale_acc_x>>ONES_BITS;
			emu.x_2last=(int64_t)adc_re*adc_re+((uint64_t)emu.x_2last*(ACC_TAO-1)>>ACC_TAO_BITS); /* 平方和滤波 */

			emu.vx+=(emu.ax+adc_re)>>1;				// 积分
			emu.ax=adc_re;
//			vx=emu.vx;

//			emu.vx_zero_last=emu.vx+((int64_t)emu.vx_zero_last*(V_ZERO_TAO-1)>>V_ZERO_TAO_BITS);/* 计算直流分量 */
//			emu.vx_zero=emu.vx_zero_last>>V_ZERO_TAO_BITS;
//			emu.vx-=emu.vx_zero;
			emu.vx=emu.vx*(V_ZERO_TAO-1)>>V_ZERO_TAO_BITS;


			emu.vx_2last=((emu.vx*emu.vx>>8)*1678>>24)+((uint64_t)emu.vx_2last*(V_TAO-1)>>V_TAO_BITS); /* 平方和滤波 */

			break;

		case Y_CH:
			toZch();

			emu.y_zero_last=adc_re+((uint64_t)emu.y_zero_last*(ACC_ZERO_TAO-1)>>ACC_ZERO_TAO_BITS); /* 计算直流分量 */
			emu.y_zero=emu.y_zero_last/ACC_ZERO_TAO;
			adc_re-=emu.y_zero;				/* 消零 */
			adc_re=adc_re*config.scale_acc_y>>ONES_BITS;
			ax0=adc_re;
			emu.y_2last=(int64_t)adc_re*adc_re+((uint64_t)emu.y_2last*(ACC_TAO-1)>>ACC_TAO_BITS); /* 平方和滤波 */

			emu.vy+=(emu.ay+adc_re)>>1;				// 积分
			emu.ay=adc_re;
			vx0=emu.vy;
	//		emu.vy_zero_last=emu.vy+((int64_t)emu.vy_zero_last*(V_ZERO_TAO-1)>>V_ZERO_TAO_BITS);/* 计算直流分量 */
	//		emu.vy_zero=emu.vy_zero_last>>V_ZERO_TAO_BITS;
	//		emu.vy-=emu.vy_zero;
			emu.vy=emu.vy*(V_ZERO_TAO-1)>>V_ZERO_TAO_BITS;


			emu.vy_2last=((emu.vy*emu.vy>>8)*1678>>24)+((uint64_t)emu.vy_2last*(V_TAO-1)>>V_TAO_BITS); /* 平方和滤波 */
			break;

		case Z_CH:
			switch(LowSpeed_AD_Channel)
			{
			case 0:
				toRes_ch();
				LowSpeed_AD_Channel = 1;
			break;
			case 1:
			    toCap_ch();
			    LowSpeed_AD_Channel = 2;
			break;
			case 2:
				toTan_ch();
				LowSpeed_AD_Channel = 3;
			break;
			case 3:
				toPt100ch();
				LowSpeed_AD_Channel = 0;
			break;
			}


			emu.z_zero_last=adc_re+((uint64_t)emu.z_zero_last*(ACC_ZERO_TAO-1)>>ACC_ZERO_TAO_BITS); /* 计算直流分量 */
			emu.z_zero=emu.z_zero_last>>ACC_ZERO_TAO_BITS;
			adc_re=adc_re-emu.z_zero;				/* 消零 */

			adc_re=adc_re*config.scale_acc_z>>ONES_BITS;
			ax=adc_re;


			emu.z_2last=(int64_t)adc_re*adc_re+((uint64_t)emu.z_2last*(ACC_TAO-1)>>ACC_TAO_BITS); /* 平方和滤波 */
			emu.vz+=(emu.az+adc_re)>>1;				// 积分

			emu.az=adc_re;
			vx=emu.vz;
//			emu.vz_zero_last=emu.vz+((int64_t)emu.vz_zero_last*(V_ZERO_TAO-1)>>V_ZERO_TAO_BITS);/* 计算直流分量 */
//			emu.vz_zero=emu.vz_zero_last>>V_ZERO_TAO_BITS;
//			emu.vz-=emu.vz_zero;
			emu.vz=emu.vz*(V_ZERO_TAO-1)>>V_ZERO_TAO_BITS;
			emu.vz_2last=((emu.vz*emu.vz>>8)*1678>>24)+((uint64_t)emu.vz_2last*(V_TAO-1)>>V_TAO_BITS); /* 平方和滤波 */
			break;

		case RES_CH:
			toXch();
			emu.res_last=(adc_re-32767)+((uint64_t)emu.res_last*(RES_TAO-1)>>RES_TAO_BITS);
			break;
		case CAP_CH:
			toXch();
			emu.cap_last=(adc_re-32767)+((uint64_t)emu.cap_last*(RES_TAO-1)>>RES_TAO_BITS);
			break;
		case TAN_CH:
			toXch();
			emu.tan_last=adc_re+((uint64_t)emu.tan_last*(RES_TAO-1)>>RES_TAO_BITS);
			break;
		case PT100_CH:
			toXch();
			emu.pt100_last=adc_re+((uint64_t)emu.pt100_last*(TEMP_TAO-1)>>TEMP_TAO_BITS); /* 计算直流分量 */
			break;
		default:
			break;
	}

	//GPIOB->PCOR = 1;   //GPIO_ClearPinsOutput(GPIOB,1<<0);
}

void updateParameter(void){
	static uint32_t l=0;
	int32_t tt;
	int32_t  mm,nn,pp;
	switch(l){
	case 0:
		{ // 加速度 X
			tt=int_sqrt(emu.x_2last>>ACC_TAO_BITS);
			parameter.acc_x=tt;
		}
		break;
	case 1:
		{ // 加速度Y
			tt=int_sqrt(emu.y_2last>>ACC_TAO_BITS);
			parameter.acc_y=tt;
		}
		break;
	case 2:
		{ // 加速度Z
			tt=int_sqrt(emu.z_2last>>ACC_TAO_BITS);
			parameter.acc_z=tt;

		}
		break;
	case 3:
		{ // 速度 X
			tt=int_sqrt(emu.vx_2last>>V_TAO_BITS);
			parameter.v_x=tt;
		}
		break;
	case 4:
		{ // 速度 Y
			tt=int_sqrt(emu.vy_2last>>V_TAO_BITS);
			parameter.v_y=tt;
		}
		break;
	case 5:
		{ // 速度 X
			tt=int_sqrt(emu.vz_2last>>V_TAO_BITS);
			parameter.v_z=tt;

		}
		break;

	case 6:
		mm = emu.res_last/RES_TAO ;
		nn = emu.cap_last/RES_TAO ;
		pp = (uint64_t)nn*1000*config.reference_res /mm ;
		pp = (uint64_t)1000*159235668 / pp ;

		parameter.cap = (pp/10 - config.distributed_cap )*config.scale_level/1000;
		parameter.cap = (int64_t)(parameter.cap *(10757 - parameter.temp * 23 /100))/10000;
		if(parameter.cap < 3000)
		{
			parameter.level = 0;
		}
		else
		{
			parameter.level = (((uint32_t)parameter.cap - 3000 - config.zero_level) * 54430)>>16;
		}



		break;
	case 7:
		//	parameter.pre=(int32_t)(emu.rv_last/PRE_TAO);
		{
			parameter.rt=1000+(((int32_t)(emu.pt100_last/TEMP_TAO)-config.zero_temp)*config.scale_temp/10000*3396>>16);
			tt=((int32_t)parameter.rt-1000)*10000/385;
			if(tt<-5000)tt=-5000;
			if(tt>15000)tt=15000;
			parameter.temp=tt;
		}
		break;
	case 8:
		tt = ((emu.tan_last / RES_TAO*180)>> 16);
		parameter.angle = 90 - tt;
		break;
	default:
		break;
	}

	l=(l+1)%10;

}



