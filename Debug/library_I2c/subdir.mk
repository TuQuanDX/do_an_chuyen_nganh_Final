################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../library_I2c/i2c-lcd.c 

OBJS += \
./library_I2c/i2c-lcd.o 

C_DEPS += \
./library_I2c/i2c-lcd.d 


# Each subdirectory must supply rules for building sources it contributes
library_I2c/i2c-lcd.o: C:/Users/ADMIN/STM32CubeIDE/workspace_1.5.1/do_an_chuyen_nganh/library_I2c/i2c-lcd.c library_I2c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ADMIN/STM32CubeIDE/workspace_1.5.1/do_an_chuyen_nganh/library_I2c" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"library_I2c/i2c-lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
