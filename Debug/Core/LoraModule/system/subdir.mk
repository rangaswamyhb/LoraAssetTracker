################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/LoraModule/system/delay.c \
../Core/LoraModule/system/fifo.c \
../Core/LoraModule/system/lr1110_adc.c \
../Core/LoraModule/system/lr1110_gpio.c \
../Core/LoraModule/system/lr1110_timer.c \
../Core/LoraModule/system/nvmm.c \
../Core/LoraModule/system/systime.c 

OBJS += \
./Core/LoraModule/system/delay.o \
./Core/LoraModule/system/fifo.o \
./Core/LoraModule/system/lr1110_adc.o \
./Core/LoraModule/system/lr1110_gpio.o \
./Core/LoraModule/system/lr1110_timer.o \
./Core/LoraModule/system/nvmm.o \
./Core/LoraModule/system/systime.o 

C_DEPS += \
./Core/LoraModule/system/delay.d \
./Core/LoraModule/system/fifo.d \
./Core/LoraModule/system/lr1110_adc.d \
./Core/LoraModule/system/lr1110_gpio.d \
./Core/LoraModule/system/lr1110_timer.d \
./Core/LoraModule/system/nvmm.d \
./Core/LoraModule/system/systime.d 


# Each subdirectory must supply rules for building sources it contributes
Core/LoraModule/system/%.o: ../Core/LoraModule/system/%.c Core/LoraModule/system/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/Git/LoraAssetMoniter/LoraAssetTracker/Core/LoraModule" -I../Core/LoraModule -I../Core/LoraModule/system -I../Core/LoraModule/boards -I../Core/LoraModule/mac -I../Core/LoraModule/mac/region -I../Core/LoraModule/peripherals -I../Core/LoraModule/peripherals/lr1110-se -I../Core/LoraModule/peripherals/atecc608a-tnglora-se -I../Core/LoraModule/peripherals/soft-se -I../Core/LoraModule/radio -I../Core/LoraModule/radio/wifi -I../Core/LoraModule/radio/lr1110_driver -I../Core/LoraModule/radio/lr1110_driver/src -I../Core/LoraModule/common -I../Core/LoraModule/common/LmHandler -I../Core/LoraModule/common/LmHandler/packages -I../STM32_WPAN/App -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-LoraModule-2f-system

clean-Core-2f-LoraModule-2f-system:
	-$(RM) ./Core/LoraModule/system/delay.d ./Core/LoraModule/system/delay.o ./Core/LoraModule/system/fifo.d ./Core/LoraModule/system/fifo.o ./Core/LoraModule/system/lr1110_adc.d ./Core/LoraModule/system/lr1110_adc.o ./Core/LoraModule/system/lr1110_gpio.d ./Core/LoraModule/system/lr1110_gpio.o ./Core/LoraModule/system/lr1110_timer.d ./Core/LoraModule/system/lr1110_timer.o ./Core/LoraModule/system/nvmm.d ./Core/LoraModule/system/nvmm.o ./Core/LoraModule/system/systime.d ./Core/LoraModule/system/systime.o

.PHONY: clean-Core-2f-LoraModule-2f-system

