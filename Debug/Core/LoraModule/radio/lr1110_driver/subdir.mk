################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/LoraModule/radio/lr1110_driver/lr1110_bootloader.c \
../Core/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.c \
../Core/LoraModule/radio/lr1110_driver/lr1110_driver_version.c \
../Core/LoraModule/radio/lr1110_driver/lr1110_gnss.c \
../Core/LoraModule/radio/lr1110_driver/lr1110_radio.c \
../Core/LoraModule/radio/lr1110_driver/lr1110_regmem.c \
../Core/LoraModule/radio/lr1110_driver/lr1110_system.c \
../Core/LoraModule/radio/lr1110_driver/lr1110_wifi.c 

OBJS += \
./Core/LoraModule/radio/lr1110_driver/lr1110_bootloader.o \
./Core/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.o \
./Core/LoraModule/radio/lr1110_driver/lr1110_driver_version.o \
./Core/LoraModule/radio/lr1110_driver/lr1110_gnss.o \
./Core/LoraModule/radio/lr1110_driver/lr1110_radio.o \
./Core/LoraModule/radio/lr1110_driver/lr1110_regmem.o \
./Core/LoraModule/radio/lr1110_driver/lr1110_system.o \
./Core/LoraModule/radio/lr1110_driver/lr1110_wifi.o 

C_DEPS += \
./Core/LoraModule/radio/lr1110_driver/lr1110_bootloader.d \
./Core/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.d \
./Core/LoraModule/radio/lr1110_driver/lr1110_driver_version.d \
./Core/LoraModule/radio/lr1110_driver/lr1110_gnss.d \
./Core/LoraModule/radio/lr1110_driver/lr1110_radio.d \
./Core/LoraModule/radio/lr1110_driver/lr1110_regmem.d \
./Core/LoraModule/radio/lr1110_driver/lr1110_system.d \
./Core/LoraModule/radio/lr1110_driver/lr1110_wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/LoraModule/radio/lr1110_driver/%.o: ../Core/LoraModule/radio/lr1110_driver/%.c Core/LoraModule/radio/lr1110_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/Git/LoraAssetMoniter/LoraAssetTracker/Core/LoraModule" -I../Core/LoraModule -I../Core/LoraModule/system -I../Core/LoraModule/boards -I../Core/LoraModule/mac -I../Core/LoraModule/mac/region -I../Core/LoraModule/peripherals -I../Core/LoraModule/peripherals/lr1110-se -I../Core/LoraModule/peripherals/atecc608a-tnglora-se -I../Core/LoraModule/peripherals/soft-se -I../Core/LoraModule/radio -I../Core/LoraModule/radio/wifi -I../Core/LoraModule/radio/lr1110_driver -I../Core/LoraModule/radio/lr1110_driver/src -I../Core/LoraModule/common -I../Core/LoraModule/common/LmHandler -I../Core/LoraModule/common/LmHandler/packages -I../STM32_WPAN/App -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-LoraModule-2f-radio-2f-lr1110_driver

clean-Core-2f-LoraModule-2f-radio-2f-lr1110_driver:
	-$(RM) ./Core/LoraModule/radio/lr1110_driver/lr1110_bootloader.d ./Core/LoraModule/radio/lr1110_driver/lr1110_bootloader.o ./Core/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.d ./Core/LoraModule/radio/lr1110_driver/lr1110_crypto_engine.o ./Core/LoraModule/radio/lr1110_driver/lr1110_driver_version.d ./Core/LoraModule/radio/lr1110_driver/lr1110_driver_version.o ./Core/LoraModule/radio/lr1110_driver/lr1110_gnss.d ./Core/LoraModule/radio/lr1110_driver/lr1110_gnss.o ./Core/LoraModule/radio/lr1110_driver/lr1110_radio.d ./Core/LoraModule/radio/lr1110_driver/lr1110_radio.o ./Core/LoraModule/radio/lr1110_driver/lr1110_regmem.d ./Core/LoraModule/radio/lr1110_driver/lr1110_regmem.o ./Core/LoraModule/radio/lr1110_driver/lr1110_system.d ./Core/LoraModule/radio/lr1110_driver/lr1110_system.o ./Core/LoraModule/radio/lr1110_driver/lr1110_wifi.d ./Core/LoraModule/radio/lr1110_driver/lr1110_wifi.o

.PHONY: clean-Core-2f-LoraModule-2f-radio-2f-lr1110_driver

