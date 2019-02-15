/*
 * kl16z64emu.h
 *
 *  Created on: Nov 24, 2017
 *      Author: zzh
 */

#ifndef EMU_KL16Z64EMU_H_
#define EMU_KL16Z64EMU_H_

#include <MKL16Z4.h>
struct EMU{
	int32_t ax;uint32_t x_2sum,x_zero,x_zero_last,x_last; uint64_t x_2last;
	int32_t ay;uint32_t y_2sum,y_zero,y_zero_last,y_last; uint64_t y_2last;
	int32_t az;uint32_t z_2sum,z_zero,z_zero_last,z_last; uint64_t z_2last;

	uint32_t rv_sum,rv_2sum,rv_zero,rv_zero_last,rv_last,res;
	uint32_t pt100_sum,pt100_2sum,pt100_zero,pt100_zero_last,pt100_last,pt100_2last;
	uint32_t res_sum,res_2sum,res_zero,res_zero_last,res_last,res_2last;
	uint32_t cap_sum,cap_2sum,cap_zero,cap_zero_last,cap_last,cap_2last;
	uint32_t tan_sum,tan_2sum,tan_zero,tan_zero_last,tan_last,tan_2last;

	int64_t vx,vx_zero,vx_zero_last,vx_2last,vx_pre;
	int64_t vy,vy_zero,vy_zero_last,vy_2last,vy_pre;
	int64_t vz,vz_zero,vz_zero_last,vz_2last,vz_pre;
};

extern struct EMU emu;
extern void updateParameter(void);
extern void initEMU();

#endif /* EMU_KL16Z64EMU_H_ */
