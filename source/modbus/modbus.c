#include "conf.h"

#ifndef DISABLE_MODBUS

uint8_t TxdBuf[TX_BUFFER_SIZE];
uint8_t RxdBuf[RX_BUFFER_SIZE];
uint8_t RxdTelBuf[RX_BUFFER_SIZE];
uint8_t TxdBytes=0;
uint8_t RxdBytes=0;
uint8_t TxdTelBytes=0;

uint8_t ReceivedTel=0;

void    OnGetData03(void);
void    OnGetData05(void);
uint16_t CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen);
uint16_t CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen);

uint8_t  isVaildTel(void)
{
	if(RxdBytes>=1)if(RxdBuf[0]!=config.addr)return(0);
	if(RxdBytes>=2)if(RxdBuf[1]!=0x03 && RxdBuf[1]!=0x05 && RxdBuf[1]!=0x06	&& RxdBuf[1]!=0x10)return(0);

	return(1);				 // �Ϸ���
}

uint8_t getTelLength(void)
{
					
	switch(RxdBuf[1])
	{
		case 0x03: return(6);
		case 0x05: return(6);
		case 0x06: return(6);
		case 0x10: return(RxdBuf[6]+7);
		default:
				return(0);
	}					
}
uint8_t isTelComplete(void)	   // =0 ������  =1 CRC Error =2 ��ȷ
{
	uint16_t  temp16;
	uint8_t  dal, dah, dat_len;

	if(RxdBytes<8)return(0);
  ////////////////
	dat_len=getTelLength();	//�����ݳ���
	if(dat_len==0)return(0);
	if(RxdBytes<(dat_len+2))return(0);

	temp16=CRC16ISR0(&RxdBuf[0],dat_len);
 
   	dal=temp16>>8;
	dah=temp16;

	if ((RxdBuf[dat_len]==dal)&&(RxdBuf[dat_len+1]==dah))
		return(2); 
	else
	{
		return(1);
	}	
}						 

uint8_t leftRxdTel(void)		//��������һλ
{
	uint8_t i;
	if(RxdBytes<1)return(0);     // �޷�����
	for	(i=1;i<RxdBytes;i++)
	{
		RxdBuf[i-1]=RxdBuf[i];		
	}
	RxdBytes--;
	return(1);					 // ����һ���ֽڳɹ�

}

 void RxdByte(uint8_t c)
{	
	uint8_t 	i;
	RxdBuf[RxdBytes]=c;
	RxdBytes++;

	switch(RxdBytes)
	{
		case 0:	break;
		case 1:
		case 2:
				while(!isVaildTel())	//������Ϸ�			 
				{
					if(!leftRxdTel())break;	  // �������ֽ�
				}
				break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:	break;
		default:		
				i=isTelComplete();
				if(i==2)
				{
					//do some thing
					for(i=0;i<RxdBytes;i++)	RxdTelBuf[i]=RxdBuf[i];
					ReceivedTel=1;
					RxdBytes=0;
				}
				else if(i==1)	 // CRC error
				{
					leftRxdTel();
					while(!isVaildTel())	//������Ϸ�			 
					{
						if(!leftRxdTel())break;
					}	
				}
				else if(i==0) //û���������
				{
				
				}
				else
				{
				}
				break;

				
		}
	}


uint8_t hasByteToTxd()		 // =1 ���ֽڴ���
{
	if(TxdBytes<TxdTelBytes)return(1);
	TxdTelBytes=0;
	TxdBytes=0;
	return(0);	
}

uint8_t  getTxdByte()   //
{
	uint8_t re;
	if(TxdBytes <TxdTelBytes)
	{
		re=TxdBuf[TxdBytes];
		TxdBytes++;
		return(re);
	}
	else
	{
		TxdTelBytes=0;
		TxdBytes=0;
		return(0);
	}
}
		




void OnGetData05()			   // ���� dout;
{
	uint8_t	i;
	for(i=0;i<8;i++)	TxdBuf[i]=RxdTelBuf[i];
 	
	if(TxdBuf[4]==0xff)
	{
//		if(TxdBuf[3]==0)	{Dout1 = 1;RelayTimeCount[0]= 200	;flag1|=0x01;}
//		if(TxdBuf[3]==1)	{Dout2 = 1;RelayTimeCount[1]= 200	;flag1|=0x02;}
//		if(TxdBuf[3]==2)	yk3=0;
//		if(TxdBuf[3]==3)	yk4=0;
	}
	TxdTelBytes = 8;
	//YkDoingflag=1;	
}
uint16_t float2uint16(float f){
	int16_t t=f;
	return(*(uint16_t*)&t);
}
extern int32_t pf;
void OnGetData03()
{	
	uint8_t 	i;
	uint16_t startaddr=(uint16_t)RxdTelBuf[2]*256+RxdTelBuf[3];
	uint16_t datanum=(uint16_t)RxdTelBuf[4]*256+RxdTelBuf[5];
	uint16_t tt;
	uint8_t *pb;
	TxdBuf[0] = config.addr;
	TxdBuf[1] = 3;//0x03;
	TxdBuf[2] = datanum*2;
	pb=&TxdBuf[3];

	for(i=0;i<datanum;i++)	{
		switch(startaddr+i){
			case 0x0800:   tt=parameter.status; break;   	/* 0800H	状态字	Uint16	R03　	定义见6.2 */
			case 0x0801:	tt=parameter.temp; 	break;    	/* 0801H	当前温度	Uint16	R03	单位0.01摄氏度	 */
			case 0x0802:	tt=parameter.level; break;		/* 0802H	当前液位高度	Uint16	R03	单位为0.1mm */
			case 0x0803:	tt=parameter.acc_x;	break;		/*0803H	当前振动加速度X(RMS)	Uint16	R03	单位为mG */
			case 0x0804:	tt=parameter.acc_y;	break; 	 	/*0804H	当前振动加速度Y(RMS)	Uint16	R03	单位为mG */
			case 0x0805:	tt=parameter.acc_z; break;	 	/* 0805H	当前振动加速度Z(RMS)	Uint16	R03	单位为mG */
			case 0x0806:	tt=parameter.v_x; 	break;		/* 0806H	当前振动速度X(RMS)	Uint16	R03	单位为0.01mm/s */
			case 0x0807:	tt=parameter.v_y;	break;		/* 0807H	当前振动速度Y(RMS)	Uint16	R03	单位为0.01mm/s */
			case 0x0808:	tt=parameter.v_z; 	break;		/* 0808H	当前振动速度Z(RMS)	Uint16	R03	单位为0.01mm/s */
			case 0x0809:	tt=parameter.cap;	break; 		/* 0809H	当前电容量	Uint16	R03	单位为pF */
			case 0x080A:	tt=parameter.rt;	break; 		/* 080AH	当前电阻	Uint16	R03	单位为	欧 */
			case 0x080B:	tt=parameter.angle;	break; 		/* 080BH	当前损耗角	Uint16	R03	单位为	度 */

			case 0x0A00:	tt=config.addr; break;  /* 0A00H	通信地址	Uint16	R03,W06	1-247, 	W06可响应广播地址(0) */
			case 0x0A01:	tt=((uint16_t)config.baundrate<<8)|config.parity; break; /*  0A01H 	通信参数	Uint16	R03,W06	高位波特率 	0:600;1:1200;2:2400;3:4800;	4:9600;	5:19200;6:38400;7:57600;8:115200;*/
			case 0x0A02:	tt=config.density;		break; 	 /* 0A02H	液体密度	Uint16	R03,W06	Kg/m3 */
			case 0x0A03:	tt=config.zero_level;		break;	/*0A03H	液位零点	Uint16	R03,W06	*/
			case 0x0A04:	tt=config.scale_level;		break;  /* 0A04H	液位scale	Uint16	R03,W06	 */
			case 0x0A05:	tt=config.zero_temp; 		break;  /* 0A05H	温度零点(0度采样值)	Uint16	R03,W06	*/
			case 0x0A06:	tt=config.scale_temp;		break; /* 0A06H	温度scale（100度采样值）	Uint16	R03,W06	*/
			case 0x0A07:	tt=emu.x_zero; 		break; /* 0A07H	加速度零点X	Uint16	R03,W06	*/
			case 0x0A08:	tt=emu.y_zero;  		break;/* 0A08H	加速度零点Y	Uint16	R03,W06	*/
			case 0x0A09:	tt=emu.z_zero;  		break; /* 0A09H	加速度零点Z	Uint16	R03,W06 */
			case 0x0A0A:	tt=config.scale_acc_x; 		break; /* 0A0AH	加速度scale X	Uint16	R03,W06	*/
			case 0x0A0B:	tt=config.scale_acc_y; 		break; /* 0A0BH	加速度scale Y	Uint16	R03,W06	*/
			case 0x0A0C:	tt=config.scale_acc_z; 		break; /* 0A0CH	加速度scale Z	Uint16	R03,W06	*/
			case 0x0A0D:	tt=config.scale_v_x;		break; /* 0A0DH	速度scale X	Uint16	R03,W06	*/
			case 0x0A0E:	tt=config.scale_v_y; 		break; /* 0A0EH	速度scale Y	Uint16	R03,W06	*/
			case 0x0A0F:	tt=config.scale_v_z; 		break; /* 0A0FH	速度scale Z	Uint16	R03,W06	*/
			case 0x0A10:	tt=config.reference_res; 		break; /* 0A10H 参考电阻	Uint16	R03,W06	*/
			case 0x0A11:	tt=config.distributed_cap; 		break;

				default:			tt=0x00;break;
			}
			*pb=tt>>8; pb++;
			*pb=tt&0xff; pb++;
	} 
	tt=CRC16ISR1(&TxdBuf[0],3+TxdBuf[2]);
	*pb=tt/256; *(pb+1)=tt%256; 
	TxdBytes=0;
	TxdTelBytes=5+datanum*2;
	
	startTxd();			   // ����һ���ж� ����������

}
void OnSetData05(void){
//	OnSetData06();

}


void OnSetData06(void)
{	
	uint8_t 	i;
	for(i=0;i<8;i++)	TxdBuf[i]=RxdTelBuf[i];
	TxdTelBytes = 8;	TxdBytes=0;
	if(TxdBuf[0]!=0)startTxd();

	{
		uint16_t reg=((uint16_t)RxdTelBuf[2]<<8)|RxdTelBuf[3];
		uint16_t v=((uint16_t)RxdTelBuf[4]<<8)|RxdTelBuf[5];
		switch (reg){
			case 0x0A00:	config.addr=v&0xff; 		break;  /* 0A00H	通信地址	Uint16	R03,W06	1-247, 	W06可响应广播地址(0) */
			case 0x0A01:	config.baundrate=RxdTelBuf[4];config.parity=RxdTelBuf[5]; break; /*  0A01H 	通信参数	Uint16	R03,W06	高位波特率 	0:600;1:1200;2:2400;3:4800;	4:9600;	5:19200;6:38400;7:57600;8:115200;*/
			case 0x0A02:	config.density=v;		break; 	 /* 0A02H	液体密度	Uint16	R03,W06	Kg/m3 */
			case 0x0A03:	config.zero_level=v;		break;	/* 80A03H	液位零点	Uint16	R03,W06	*/
			case 0x0A04:	config.scale_level=v;		break;  /* 0A04H	液位scale	Uint16	R03,W06	 */
			case 0x0A05:	config.zero_temp=v; 		break;  /* 0A05H	温度零点(0度采样值)	Uint16	R03,W06	*/
			case 0x0A06:	config.scale_temp=v;		break; /* 0A06H	温度scale（100度采样值）	Uint16	R03,W06	*/
			case 0x0A07:	config.zero_acc_x=v; 		break; /* 0A07H	加速度零点X	Uint16	R03,W06	*/
			case 0x0A08:	config.zero_acc_y=v; 		break;/* 0A08H	加速度零点Y	Uint16	R03,W06	*/
			case 0x0A09:	config.zero_acc_z=v; 		break; /* 0A09H	加速度零点Z	Uint16	R03,W06 */
			case 0x0A0A:	config.scale_acc_x=v; 		break; /* 0A0AH	加速度scale X	Uint16	R03,W06	*/
			case 0x0A0B:	config.scale_acc_y=v; 		break; /* 0A0BH	加速度scale Y	Uint16	R03,W06	*/
			case 0x0A0C:	config.scale_acc_z=v; 		break; /* 0A0CH	加速度scale Z	Uint16	R03,W06	*/
			case 0x0A0D:	config.scale_v_x=v;		break; /* 0A0DH	速度scale X	Uint16	R03,W06	*/
			case 0x0A0E:	config.scale_v_y=v; 		break; /* 0A0EH	速度scale Y	Uint16	R03,W06	*/
			case 0x0A0F:	config.scale_v_z=v; 		break; /* 0A0FH	速度scale Z	Uint16	R03,W06	*/
			case 0x0A10:	config.reference_res=v; 		break;
			case 0x0A11:	config.distributed_cap=v; 		break;
			default:			break;
		}

		saveConfig();
	}
		
	return;
}



const     uint8_t  auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};


const    	 uint8_t  auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};

uint16_t CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen) 	//80109
{
   uint8_t uchCRCHi=0xFF; // ??????
   uint8_t uchCRCLo=0xFF; // ??????
   uint8_t uIndex ;      //?CRC?
   while (usDataLen--) //???????
   {
	uIndex = uchCRCHi^*puchMsg++ ; //??CRC 
	uchCRCHi=uchCRCLo^auchCRCHi[uIndex] ;
	uchCRCLo=auchCRCLo[uIndex] ;
   }
	return ((((uint16_t)uchCRCHi )<< 8 )| uchCRCLo);
}
uint16_t CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen)
{
   uint8_t uchCRCHi = 0xFF ; // ??????
   uint8_t uchCRCLo = 0xFF ; // ??????
   uint8_t uIndex ;          //?CRC?
   while (usDataLen--)     //???????
   {
    uIndex = uchCRCHi ^ *puchMsg++ ; //??CRC 
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
    uchCRCLo = auchCRCLo[uIndex] ;
   }
	return ((((uint16_t)uchCRCHi )<< 8 )| uchCRCLo);
}

void modbusLoop()
{	
	static uint8_t comflag_count=0;

	if(comflag_count>0)
	{
		comflag_count--;
		if(comflag_count==0)	{
		}
	}	
	
	disableInt();
	if(ReceivedTel)
	{
		ReceivedTel=0;
		enableInt();
 		
		switch(getRxdTelCommand())
		{
          case 0x03:
                OnGetData03();		// �����ּĴ���
                break;
        case 0x05:                   //д��Ȧ
  //              OnSetData05();
  //              break;	
		case 0x06:
				OnSetData06();				//д�Ĵ���
                break;
		}
	}
	else
	{	
		enableInt();
	}
}
void testTxd(void){
	int i;
	for(i=0;i<8;i++)	TxdBuf[i]=i+1;
	TxdTelBytes = 8;	TxdBytes=0;
	if(TxdBuf[0]!=0)startTxd();
}
#endif
