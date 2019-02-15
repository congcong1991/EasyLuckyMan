################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/LQC2654.c \
../source/conf.c \
../source/kl16z64.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/LQC2654.o \
./source/conf.o \
./source/kl16z64.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/LQC2654.d \
./source/conf.d \
./source/kl16z64.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -DNDEBUG -DCPU_MKL16Z64VFT4 -DCPU_MKL16Z64VFT4_cm0plus -I"E:\shibao\workspace\workspace\LQC2654\board" -I"E:\shibao\workspace\workspace\LQC2654\source" -I"E:\shibao\workspace\workspace\LQC2654" -I"E:\shibao\workspace\workspace\LQC2654\drivers" -I"E:\shibao\workspace\workspace\LQC2654\utilities" -I"E:\shibao\workspace\workspace\LQC2654\startup" -I"E:\shibao\workspace\workspace\LQC2654\CMSIS" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


