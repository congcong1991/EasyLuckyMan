################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/modbus/modbus.c 

OBJS += \
./source/modbus/modbus.o 

C_DEPS += \
./source/modbus/modbus.d 


# Each subdirectory must supply rules for building sources it contributes
source/modbus/%.o: ../source/modbus/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -DDEBUG -DCPU_MKL16Z64VFT4 -DCPU_MKL16Z64VFT4_cm0plus -I"D:\NB-IOT\LQC2664\board" -I"D:\NB-IOT\LQC2664\source" -I"D:\NB-IOT\LQC2664" -I"D:\NB-IOT\LQC2664\drivers" -I"D:\NB-IOT\LQC2664\utilities" -I"D:\NB-IOT\LQC2664\startup" -I"D:\NB-IOT\LQC2664\CMSIS" -O3 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


