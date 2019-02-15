/*
 * conf.h
 *
 *  Created on: Nov 20, 2017
 *      Author: zzh
 */
#ifndef CONF_H_
#define CONF_H_

//#define NDEBUG


#include "MKL16Z4.h"
#include "main.h"
#include "modbus/modbus.h"
#include "eeprom/kl16z64_flash.h"
#include "pga2311/pga2311.h"
#include "emu/kl16z64emu.h"
#include <fsl_gpio.h>
#include <fsl_port.h>

#define enableInt() { __enable_irq();}
#define disableInt() {__disable_irq();}


#endif /* CONF_H_ */
