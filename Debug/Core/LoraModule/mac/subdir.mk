################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/LoraModule/mac/LoRaMac.c \
../Core/LoraModule/mac/LoRaMacAdr.c \
../Core/LoraModule/mac/LoRaMacClassB.c \
../Core/LoraModule/mac/LoRaMacCommands.c \
../Core/LoraModule/mac/LoRaMacConfirmQueue.c \
../Core/LoraModule/mac/LoRaMacCrypto.c \
../Core/LoraModule/mac/LoRaMacParser.c \
../Core/LoraModule/mac/LoRaMacSerializer.c 

OBJS += \
./Core/LoraModule/mac/LoRaMac.o \
./Core/LoraModule/mac/LoRaMacAdr.o \
./Core/LoraModule/mac/LoRaMacClassB.o \
./Core/LoraModule/mac/LoRaMacCommands.o \
./Core/LoraModule/mac/LoRaMacConfirmQueue.o \
./Core/LoraModule/mac/LoRaMacCrypto.o \
./Core/LoraModule/mac/LoRaMacParser.o \
./Core/LoraModule/mac/LoRaMacSerializer.o 

C_DEPS += \
./Core/LoraModule/mac/LoRaMac.d \
./Core/LoraModule/mac/LoRaMacAdr.d \
./Core/LoraModule/mac/LoRaMacClassB.d \
./Core/LoraModule/mac/LoRaMacCommands.d \
./Core/LoraModule/mac/LoRaMacConfirmQueue.d \
./Core/LoraModule/mac/LoRaMacCrypto.d \
./Core/LoraModule/mac/LoRaMacParser.d \
./Core/LoraModule/mac/LoRaMacSerializer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/LoraModule/mac/%.o: ../Core/LoraModule/mac/%.c Core/LoraModule/mac/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/LoraPingPong/Core/LoraModule" -I../Core/LoraModule -I../Core/LoraModule/system -I../Core/LoraModule/boards -I../Core/LoraModule/mac -I../Core/LoraModule/mac/region -I../Core/LoraModule/peripherals -I../Core/LoraModule/peripherals/lr1110-se -I../Core/LoraModule/peripherals/atecc608a-tnglora-se -I../Core/LoraModule/peripherals/soft-se -I../Core/LoraModule/radio -I../Core/LoraModule/radio/wifi -I../Core/LoraModule/radio/lr1110_driver -I../Core/LoraModule/radio/lr1110_driver/src -I../Core/LoraModule/common -I../Core/LoraModule/common/LmHandler -I../Core/LoraModule/common/LmHandler/packages -I../STM32_WPAN/App -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-LoraModule-2f-mac

clean-Core-2f-LoraModule-2f-mac:
	-$(RM) ./Core/LoraModule/mac/LoRaMac.d ./Core/LoraModule/mac/LoRaMac.o ./Core/LoraModule/mac/LoRaMacAdr.d ./Core/LoraModule/mac/LoRaMacAdr.o ./Core/LoraModule/mac/LoRaMacClassB.d ./Core/LoraModule/mac/LoRaMacClassB.o ./Core/LoraModule/mac/LoRaMacCommands.d ./Core/LoraModule/mac/LoRaMacCommands.o ./Core/LoraModule/mac/LoRaMacConfirmQueue.d ./Core/LoraModule/mac/LoRaMacConfirmQueue.o ./Core/LoraModule/mac/LoRaMacCrypto.d ./Core/LoraModule/mac/LoRaMacCrypto.o ./Core/LoraModule/mac/LoRaMacParser.d ./Core/LoraModule/mac/LoRaMacParser.o ./Core/LoraModule/mac/LoRaMacSerializer.d ./Core/LoraModule/mac/LoRaMacSerializer.o

.PHONY: clean-Core-2f-LoraModule-2f-mac

