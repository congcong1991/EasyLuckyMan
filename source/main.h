/*
 * main.h
 *
 *  Created on: Nov 20, 2017
 *      Author: zzh
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "conf.h"

struct CONFIG{
	uint16_t vaildsign;
	uint8_t addr;	 /* 0A00H	通信地址	Uint16	R03,W06	1-247, 	W06可响应广播地址(0) */
	uint8_t baundrate; /*  0A01H 	通信参数	Uint16	R03,W06	高位波特率 	0:600;1:1200;2:2400;3:4800;	4:9600;	5:19200;6:38400;7:57600;8:115200;*/
	uint8_t parity;  /* 0A01H  低位数据格式:  0:8n1; 1:8o1; 2:8e1 */
	uint16_t density; /* 0A02H	液体密度	Uint16	R03,W06	Kg/m3 */
	uint16_t zero_level; 	/* 80A03H	液位零点	Uint16	R03,W06	*/
	uint16_t scale_level;  /* 0A04H	液位scale	Uint16	R03,W06	 */
	uint16_t zero_temp;  /* 0A05H	温度零点(0度采样值)	Uint16	R03,W06	*/
	uint16_t scale_temp; /* 0A06H	温度scale（100度采样值）	Uint16	R03,W06	*/
	uint16_t zero_acc_x; /* 0A07H	加速度零矢量X	Uint16	R03,W06	*/
	uint16_t zero_acc_y; /* 0A08H	加速度零矢量Y	Uint16	R03,W06	*/
	uint16_t zero_acc_z; /* 0A09H	加速度零矢量Z	Uint16	R03,W06 */
	uint16_t scale_acc_x; /* 0A0AH	加速度scale X	Uint16	R03,W06	*/
	uint16_t scale_acc_y; /* 0A0BH	加速度scale Y	Uint16	R03,W06	*/
	uint16_t scale_acc_z; /* 0A0CH	加速度scale Z	Uint16	R03,W06	*/
	uint16_t scale_v_x; /* 0A0DH	速度scale X	Uint16	R03,W06	*/
	uint16_t scale_v_y; /* 0A0EH	速度scale Y	Uint16	R03,W06	*/
	uint16_t scale_v_z; /* 0A0FH	速度scale Z	Uint16	R03,W06	*/
	uint16_t reference_res ; /* 0A10H   参考电阻阻值(单位0.1K) R03,W06*/
	uint16_t distributed_cap; /*0A11H   分布电容（单位0.01pF）*/
};

struct PARAMETER{
	uint16_t status; /* 0800H	状态字	Uint16	R03　	定义见6.2 */
	int16_t temp;   /* 0801H	当前温度	Uint16	R03	单位0.01摄氏度	 */
	uint16_t level; /* 0802H	当前液位高度	Uint16	R03	单位为0.1mm */
	uint16_t acc_x; /*0803H	当前振动加速度X(RMS)	Uint16	R03	单位为mG */
	uint16_t acc_y; /*0804H	当前振动加速度Y(RMS)	Uint16	R03	单位为mG */
	uint16_t acc_z; /* 0805H	当前振动加速度Z(RMS)	Uint16	R03	单位为mG */
	uint16_t v_x; /* 0806H	当前振动速度X(RMS)	Uint16	R03	单位为0.01mm/s */
	uint16_t v_y; /* 0807H	当前振动速度Y(RMS)	Uint16	R03	单位为0.01mm/s */
	uint16_t v_z; /* 0808H	当前振动速度Z(RMS)	Uint16	R03	单位为0.01mm/s */
	//uint16_t pre; /* 0809H	当前油压	Uint16	R03	单位为Pa */
	uint16_t cap;/*0809H 当前电容容量         Uint16	R03	单位为pF*/
	uint16_t rt; /*080AH  单位　欧　*/
	int16_t angle;/*080BH  损耗角 单位 度*/
};

extern struct CONFIG config;
extern struct PARAMETER parameter;

#endif /* MAIN_H_ */
