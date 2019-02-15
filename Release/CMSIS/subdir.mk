################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL16Z4.c 

OBJS += \
./CMSIS/system_MKL16Z4.o 

C_DEPS += \
./CMSIS/system_MKL16Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -DNDEBUG -DCPU_MKL16Z64VFT4 -DCPU_MKL16Z64VFT4_cm0plus -I"E:\shibao\workspace\workspace\LQC2654\board" -I"E:\shibao\workspace\workspace\LQC2654\source" -I"E:\shibao\workspace\workspace\LQC2654" -I"E:\shibao\workspace\workspace\LQC2654\drivers" -I"E:\shibao\workspace\workspace\LQC2654\utilities" -I"E:\shibao\workspace\workspace\LQC2654\startup" -I"E:\shibao\workspace\workspace\LQC2654\CMSIS" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


