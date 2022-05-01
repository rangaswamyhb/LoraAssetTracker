################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/LoraModule/system/delay.c \
../Application/User/LoraModule/system/fifo.c \
../Application/User/LoraModule/system/lr1110_adc.c \
../Application/User/LoraModule/system/lr1110_gpio.c \
../Application/User/LoraModule/system/lr1110_timer.c \
../Application/User/LoraModule/system/nvmm.c \
../Application/User/LoraModule/system/systime.c 

OBJS += \
./Application/User/LoraModule/system/delay.o \
./Application/User/LoraModule/system/fifo.o \
./Application/User/LoraModule/system/lr1110_adc.o \
./Application/User/LoraModule/system/lr1110_gpio.o \
./Application/User/LoraModule/system/lr1110_timer.o \
./Application/User/LoraModule/system/nvmm.o \
./Application/User/LoraModule/system/systime.o 

C_DEPS += \
./Application/User/LoraModule/system/delay.d \
./Application/User/LoraModule/system/fifo.d \
./Application/User/LoraModule/system/lr1110_adc.d \
./Application/User/LoraModule/system/lr1110_gpio.d \
./Application/User/LoraModule/system/lr1110_timer.d \
./Application/User/LoraModule/system/nvmm.d \
./Application/User/LoraModule/system/systime.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/LoraModule/system/%.o: ../Application/User/LoraModule/system/%.c Application/User/LoraModule/system/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WBXX_NUCLEO -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../../Core/Inc -I../../STM32_WPAN/App -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/NUCLEO-WB15CC -I../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../Utilities/lpm/tiny_lpm -I../../Middlewares/ST/STM32_WPAN -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../Middlewares/ST/STM32_WPAN/utilities -I../../Middlewares/ST/STM32_WPAN/ble/core -I../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../Utilities/sequencer -I../../Middlewares/ST/STM32_WPAN/ble -I../../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/Git/LoraAssetMoniter/LoraAssetTracker/Projects/NUCLEO-WB15CC/Applications/BLE/LoraAssetTracker/STM32CubeIDE/Application/User/LoraModule" -I../Application/User/LoraModule/ -I../Application/User/LoraModule/boards -I../Application/User/LoraModule/mac -I../Application/User/LoraModule/mac/region -I../Application/User/LoraModule/peripherals -I../Application/User/LoraModule/peripherals/lr1110-se -I../Application/User/LoraModule/radio -I../Application/User/LoraModule/radio/lr1110_driver -I../Application/User/LoraModule/radio/wifi -I../Application/User/LoraModule/radio/gnss -I../Application/User/LoraModule/system -I../Application/User/Core -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-LoraModule-2f-system

clean-Application-2f-User-2f-LoraModule-2f-system:
	-$(RM) ./Application/User/LoraModule/system/delay.d ./Application/User/LoraModule/system/delay.o ./Application/User/LoraModule/system/fifo.d ./Application/User/LoraModule/system/fifo.o ./Application/User/LoraModule/system/lr1110_adc.d ./Application/User/LoraModule/system/lr1110_adc.o ./Application/User/LoraModule/system/lr1110_gpio.d ./Application/User/LoraModule/system/lr1110_gpio.o ./Application/User/LoraModule/system/lr1110_timer.d ./Application/User/LoraModule/system/lr1110_timer.o ./Application/User/LoraModule/system/nvmm.d ./Application/User/LoraModule/system/nvmm.o ./Application/User/LoraModule/system/systime.d ./Application/User/LoraModule/system/systime.o

.PHONY: clean-Application-2f-User-2f-LoraModule-2f-system

