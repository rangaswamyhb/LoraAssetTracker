################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/LoraModule/mac/LoRaMac.c \
../Application/User/LoraModule/mac/LoRaMacAdr.c \
../Application/User/LoraModule/mac/LoRaMacClassB.c \
../Application/User/LoraModule/mac/LoRaMacCommands.c \
../Application/User/LoraModule/mac/LoRaMacConfirmQueue.c \
../Application/User/LoraModule/mac/LoRaMacCrypto.c \
../Application/User/LoraModule/mac/LoRaMacParser.c \
../Application/User/LoraModule/mac/LoRaMacSerializer.c 

OBJS += \
./Application/User/LoraModule/mac/LoRaMac.o \
./Application/User/LoraModule/mac/LoRaMacAdr.o \
./Application/User/LoraModule/mac/LoRaMacClassB.o \
./Application/User/LoraModule/mac/LoRaMacCommands.o \
./Application/User/LoraModule/mac/LoRaMacConfirmQueue.o \
./Application/User/LoraModule/mac/LoRaMacCrypto.o \
./Application/User/LoraModule/mac/LoRaMacParser.o \
./Application/User/LoraModule/mac/LoRaMacSerializer.o 

C_DEPS += \
./Application/User/LoraModule/mac/LoRaMac.d \
./Application/User/LoraModule/mac/LoRaMacAdr.d \
./Application/User/LoraModule/mac/LoRaMacClassB.d \
./Application/User/LoraModule/mac/LoRaMacCommands.d \
./Application/User/LoraModule/mac/LoRaMacConfirmQueue.d \
./Application/User/LoraModule/mac/LoRaMacCrypto.d \
./Application/User/LoraModule/mac/LoRaMacParser.d \
./Application/User/LoraModule/mac/LoRaMacSerializer.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/LoraModule/mac/%.o: ../Application/User/LoraModule/mac/%.c Application/User/LoraModule/mac/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WBXX_NUCLEO -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../../Core/Inc -I../../STM32_WPAN/App -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/NUCLEO-WB15CC -I../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../Utilities/lpm/tiny_lpm -I../../Middlewares/ST/STM32_WPAN -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../Middlewares/ST/STM32_WPAN/utilities -I../../Middlewares/ST/STM32_WPAN/ble/core -I../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../Utilities/sequencer -I../../Middlewares/ST/STM32_WPAN/ble -I../../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/ExampleCode/STM32CubeWB-master/STM32CubeWB-master/Projects/NUCLEO-WB15CC/Applications/BLE/BLE_p2pServer/STM32CubeIDE/Application/User/LoraModule" -I../Application/User/LoraModule/ -I../Application/User/LoraModule/boards -I../Application/User/LoraModule/mac -I../Application/User/LoraModule/mac/region -I../Application/User/LoraModule/peripherals -I../Application/User/LoraModule/peripherals/lr1110-se -I../Application/User/LoraModule/radio -I../Application/User/LoraModule/radio/lr1110_driver -I../Application/User/LoraModule/radio/wifi -I../Application/User/LoraModule/radio/gnss -I../Application/User/LoraModule/system -I../Application/User/Core -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-LoraModule-2f-mac

clean-Application-2f-User-2f-LoraModule-2f-mac:
	-$(RM) ./Application/User/LoraModule/mac/LoRaMac.d ./Application/User/LoraModule/mac/LoRaMac.o ./Application/User/LoraModule/mac/LoRaMacAdr.d ./Application/User/LoraModule/mac/LoRaMacAdr.o ./Application/User/LoraModule/mac/LoRaMacClassB.d ./Application/User/LoraModule/mac/LoRaMacClassB.o ./Application/User/LoraModule/mac/LoRaMacCommands.d ./Application/User/LoraModule/mac/LoRaMacCommands.o ./Application/User/LoraModule/mac/LoRaMacConfirmQueue.d ./Application/User/LoraModule/mac/LoRaMacConfirmQueue.o ./Application/User/LoraModule/mac/LoRaMacCrypto.d ./Application/User/LoraModule/mac/LoRaMacCrypto.o ./Application/User/LoraModule/mac/LoRaMacParser.d ./Application/User/LoraModule/mac/LoRaMacParser.o ./Application/User/LoraModule/mac/LoRaMacSerializer.d ./Application/User/LoraModule/mac/LoRaMacSerializer.o

.PHONY: clean-Application-2f-User-2f-LoraModule-2f-mac

