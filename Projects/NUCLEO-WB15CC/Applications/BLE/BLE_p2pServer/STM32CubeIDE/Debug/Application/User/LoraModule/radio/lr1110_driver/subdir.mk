################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_bootloader.c \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.c \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_driver_version.c \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_gnss.c \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_radio.c \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_regmem.c \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_system.c \
../Application/User/LoraModule/radio/lr1110_driver/lr1110_wifi.c 

OBJS += \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_bootloader.o \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.o \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_driver_version.o \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_gnss.o \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_radio.o \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_regmem.o \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_system.o \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_wifi.o 

C_DEPS += \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_bootloader.d \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.d \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_driver_version.d \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_gnss.d \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_radio.d \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_regmem.d \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_system.d \
./Application/User/LoraModule/radio/lr1110_driver/lr1110_wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/LoraModule/radio/lr1110_driver/%.o: ../Application/User/LoraModule/radio/lr1110_driver/%.c Application/User/LoraModule/radio/lr1110_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WBXX_NUCLEO -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../../Core/Inc -I../../STM32_WPAN/App -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/NUCLEO-WB15CC -I../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../Utilities/lpm/tiny_lpm -I../../Middlewares/ST/STM32_WPAN -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../Middlewares/ST/STM32_WPAN/utilities -I../../Middlewares/ST/STM32_WPAN/ble/core -I../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../Utilities/sequencer -I../../Middlewares/ST/STM32_WPAN/ble -I../../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/ExampleCode/STM32CubeWB-master/STM32CubeWB-master/Projects/NUCLEO-WB15CC/Applications/BLE/BLE_p2pServer/STM32CubeIDE/Application/User/LoraModule" -I../Application/User/LoraModule/ -I../Application/User/LoraModule/boards -I../Application/User/LoraModule/mac -I../Application/User/LoraModule/mac/region -I../Application/User/LoraModule/peripherals -I../Application/User/LoraModule/peripherals/lr1110-se -I../Application/User/LoraModule/radio -I../Application/User/LoraModule/radio/lr1110_driver -I../Application/User/LoraModule/radio/wifi -I../Application/User/LoraModule/radio/gnss -I../Application/User/LoraModule/system -I../Application/User/Core -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-LoraModule-2f-radio-2f-lr1110_driver

clean-Application-2f-User-2f-LoraModule-2f-radio-2f-lr1110_driver:
	-$(RM) ./Application/User/LoraModule/radio/lr1110_driver/lr1110_bootloader.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_bootloader.o ./Application/User/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.o ./Application/User/LoraModule/radio/lr1110_driver/lr1110_driver_version.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_driver_version.o ./Application/User/LoraModule/radio/lr1110_driver/lr1110_gnss.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_gnss.o ./Application/User/LoraModule/radio/lr1110_driver/lr1110_radio.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_radio.o ./Application/User/LoraModule/radio/lr1110_driver/lr1110_regmem.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_regmem.o ./Application/User/LoraModule/radio/lr1110_driver/lr1110_system.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_system.o ./Application/User/LoraModule/radio/lr1110_driver/lr1110_wifi.d ./Application/User/LoraModule/radio/lr1110_driver/lr1110_wifi.o

.PHONY: clean-Application-2f-User-2f-LoraModule-2f-radio-2f-lr1110_driver

