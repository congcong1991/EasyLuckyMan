/*
 * conf.c
 *
 *  Created on: Nov 20, 2017
 *      Author: zzh
 */
#include "conf.h"

struct CONFIG config={
		0x55AA,	/* uint16_t vaildsign; */
		1,/* uint8_t addr;	*/  /* 0A00H	通信地址	Uint16	R03,W06	1-247, 	W06可响应广播地址(0) */
		4,/* uint8_t baundrate;*/ /*  0A01H 	通信参数	Uint16	R03,W06	高位波特率 	0:600;1:1200;2:2400;3:4800;	4:9600;	5:19200;6:38400;7:57600;8:115200;*/
		0,/* uint8_t parity; */  /* 0A01H  低位数据格式:  0:8n1; 1:8o1; 2:8e1 */
		800, /* uint16_t density;*/ /* 0A02H	液体密度	Uint16	R03,W06	Kg/m3 */
		0, /* uint16_t zero_level; */	/* 80A03H	液位零点	Uint16	R03,W06	*/
		1000, /* uint16_t scale_level; */  /* 0A04H	液位scale	Uint16	R03,W06	 */
		32768, /* uint16_t zero_temp; */  /* 0A05H	温度零点(0度采样值)	Uint16	R03,W06	*/
		10000, /* uint16_t scale_temp; */ /* 0A06H	温度scale 	Uint16	R03,W06	*/
		0, /* uint16_t zero_acc_x; */ /* 0A07H	加速度零点X	Uint16	R03,W06	*/
		0, /* uint16_t zero_acc_y; */ /* 0A08H	加速度零点Y	Uint16	R03,W06	*/
		0, /* uint16_t zero_acc_z; */ /* 0A09H	加速度零点Z	Uint16	R03,W06 */
		1000, /* uint16_t scale_acc_x; */ /* 0A0AH	加速度scale X	Uint16	R03,W06	*/
		1000, /* uint16_t scale_acc_y; */ /* 0A0BH	加速度scale Y	Uint16	R03,W06	*/
		1000, /* uint16_t scale_acc_z; */ /* 0A0CH	加速度scale Z	Uint16	R03,W06	*/
		1000, /* uint16_t scale_v_x;*/  /* 0A0DH	速度scale X	Uint16	R03,W06	*/
		1000, /* uint16_t scale_v_y; */ /* 0A0EH	速度scale Y	Uint16	R03,W06	*/
		1000, /* uint16_t scale_v_z; */ /* 0A0FH	速度scale Z	Uint16	R03,W06	*/
	    1000,
		0,


};

struct PARAMETER parameter={
	0, /* uint16_t status;*/  /* 0800H	状态字	Uint16	R03　	定义见6.2 */
	2500, /*uint16_t temp;*/   /* 0801H	当前温度	Uint16	R03	单位0.01摄氏度	 */
	100, /*uint16_t level;*/ /* 0802H	当前液位高度	Uint16	R03	单位为0.1mm */
	0, /*uint16_t acc_x;*/ /*0803H	当前振动加速度X(RMS)	Uint16	R03	单位为mG */
	0, /* uint16_t acc_y; */ /*0804H	当前振动加速度Y(RMS)	Uint16	R03	单位为mG */
	0, /* uint16_t acc_z; */ /* 0805H	当前振动加速度Z(RMS)	Uint16	R03	单位为mG */
	0, /*uint16_t v_x;*/ /* 0806H	当前振动速度X(RMS)	Uint16	R03	单位为0.01mm/s */
	0, /* uint16_t v_y; */ /* 0807H	当前振动速度Y(RMS)	Uint16	R03	单位为0.01mm/s */
	0, /* uint16_t v_z; */ /* 0808H	当前振动速度Z(RMS)	Uint16	R03	单位为0.01mm/s */
	0, /*uint16_t pre; */ /* 0809H	当前电容量	Uint16	R03	单位为pF */
	0, /* Rt */
	0,
};
