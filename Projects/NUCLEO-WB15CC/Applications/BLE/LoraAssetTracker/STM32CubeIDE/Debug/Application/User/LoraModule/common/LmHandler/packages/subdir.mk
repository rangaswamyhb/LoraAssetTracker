################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/LoraModule/common/LmHandler/packages/FragDecoder.c \
../Application/User/LoraModule/common/LmHandler/packages/LmhpClockSync.c \
../Application/User/LoraModule/common/LmHandler/packages/LmhpCompliance.c \
../Application/User/LoraModule/common/LmHandler/packages/LmhpFragmentation.c \
../Application/User/LoraModule/common/LmHandler/packages/LmhpRemoteMcastSetup.c 

OBJS += \
./Application/User/LoraModule/common/LmHandler/packages/FragDecoder.o \
./Application/User/LoraModule/common/LmHandler/packages/LmhpClockSync.o \
./Application/User/LoraModule/common/LmHandler/packages/LmhpCompliance.o \
./Application/User/LoraModule/common/LmHandler/packages/LmhpFragmentation.o \
./Application/User/LoraModule/common/LmHandler/packages/LmhpRemoteMcastSetup.o 

C_DEPS += \
./Application/User/LoraModule/common/LmHandler/packages/FragDecoder.d \
./Application/User/LoraModule/common/LmHandler/packages/LmhpClockSync.d \
./Application/User/LoraModule/common/LmHandler/packages/LmhpCompliance.d \
./Application/User/LoraModule/common/LmHandler/packages/LmhpFragmentation.d \
./Application/User/LoraModule/common/LmHandler/packages/LmhpRemoteMcastSetup.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/LoraModule/common/LmHandler/packages/%.o: ../Application/User/LoraModule/common/LmHandler/packages/%.c Application/User/LoraModule/common/LmHandler/packages/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WBXX_NUCLEO -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../../Core/Inc -I../../STM32_WPAN/App -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/NUCLEO-WB15CC -I../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../Utilities/lpm/tiny_lpm -I../../Middlewares/ST/STM32_WPAN -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../Middlewares/ST/STM32_WPAN/utilities -I../../Middlewares/ST/STM32_WPAN/ble/core -I../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../Utilities/sequencer -I../../Middlewares/ST/STM32_WPAN/ble -I../../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/ExampleCode/STM32CubeWB-master/STM32CubeWB-master/Projects/NUCLEO-WB15CC/Applications/BLE/BLE_p2pServer/STM32CubeIDE/Application/User/LoraModule" -I../Application/User/LoraModule/ -I../Application/User/LoraModule/boards -I../Application/User/LoraModule/mac -I../Application/User/LoraModule/mac/region -I../Application/User/LoraModule/peripherals -I../Application/User/LoraModule/peripherals/lr1110-se -I../Application/User/LoraModule/radio -I../Application/User/LoraModule/radio/lr1110_driver -I../Application/User/LoraModule/radio/wifi -I../Application/User/LoraModule/system -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-LoraModule-2f-common-2f-LmHandler-2f-packages

clean-Application-2f-User-2f-LoraModule-2f-common-2f-LmHandler-2f-packages:
	-$(RM) ./Application/User/LoraModule/common/LmHandler/packages/FragDecoder.d ./Application/User/LoraModule/common/LmHandler/packages/FragDecoder.o ./Application/User/LoraModule/common/LmHandler/packages/LmhpClockSync.d ./Application/User/LoraModule/common/LmHandler/packages/LmhpClockSync.o ./Application/User/LoraModule/common/LmHandler/packages/LmhpCompliance.d ./Application/User/LoraModule/common/LmHandler/packages/LmhpCompliance.o ./Application/User/LoraModule/common/LmHandler/packages/LmhpFragmentation.d ./Application/User/LoraModule/common/LmHandler/packages/LmhpFragmentation.o ./Application/User/LoraModule/common/LmHandler/packages/LmhpRemoteMcastSetup.d ./Application/User/LoraModule/common/LmHandler/packages/LmhpRemoteMcastSetup.o

.PHONY: clean-Application-2f-User-2f-LoraModule-2f-common-2f-LmHandler-2f-packages

