################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/LoraModule/mac/region/Region.c \
../Core/LoraModule/mac/region/RegionAS923.c \
../Core/LoraModule/mac/region/RegionBaseUS.c \
../Core/LoraModule/mac/region/RegionCN779.c \
../Core/LoraModule/mac/region/RegionCommon.c \
../Core/LoraModule/mac/region/RegionEU433.c \
../Core/LoraModule/mac/region/RegionEU868.c \
../Core/LoraModule/mac/region/RegionIN865.c \
../Core/LoraModule/mac/region/RegionKR920.c \
../Core/LoraModule/mac/region/RegionRU864.c 

OBJS += \
./Core/LoraModule/mac/region/Region.o \
./Core/LoraModule/mac/region/RegionAS923.o \
./Core/LoraModule/mac/region/RegionBaseUS.o \
./Core/LoraModule/mac/region/RegionCN779.o \
./Core/LoraModule/mac/region/RegionCommon.o \
./Core/LoraModule/mac/region/RegionEU433.o \
./Core/LoraModule/mac/region/RegionEU868.o \
./Core/LoraModule/mac/region/RegionIN865.o \
./Core/LoraModule/mac/region/RegionKR920.o \
./Core/LoraModule/mac/region/RegionRU864.o 

C_DEPS += \
./Core/LoraModule/mac/region/Region.d \
./Core/LoraModule/mac/region/RegionAS923.d \
./Core/LoraModule/mac/region/RegionBaseUS.d \
./Core/LoraModule/mac/region/RegionCN779.d \
./Core/LoraModule/mac/region/RegionCommon.d \
./Core/LoraModule/mac/region/RegionEU433.d \
./Core/LoraModule/mac/region/RegionEU868.d \
./Core/LoraModule/mac/region/RegionIN865.d \
./Core/LoraModule/mac/region/RegionKR920.d \
./Core/LoraModule/mac/region/RegionRU864.d 


# Each subdirectory must supply rules for building sources it contributes
Core/LoraModule/mac/region/%.o: ../Core/LoraModule/mac/region/%.c Core/LoraModule/mac/region/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB15xx -DUSE_MODEM_LORA -DLR1110MB1XXS -DREGION_IN865 -DSOFT_SE -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"D:/LoraEdge/Code/Git/LoraAssetMoniter/LoraAssetTracker/Core/LoraModule" -I../Core/LoraModule -I../Core/LoraModule/system -I../Core/LoraModule/boards -I../Core/LoraModule/mac -I../Core/LoraModule/mac/region -I../Core/LoraModule/peripherals -I../Core/LoraModule/peripherals/lr1110-se -I../Core/LoraModule/peripherals/atecc608a-tnglora-se -I../Core/LoraModule/peripherals/soft-se -I../Core/LoraModule/radio -I../Core/LoraModule/radio/wifi -I../Core/LoraModule/radio/lr1110_driver -I../Core/LoraModule/radio/lr1110_driver/src -I../Core/LoraModule/common -I../Core/LoraModule/common/LmHandler -I../Core/LoraModule/common/LmHandler/packages -I../STM32_WPAN/App -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-LoraModule-2f-mac-2f-region

clean-Core-2f-LoraModule-2f-mac-2f-region:
	-$(RM) ./Core/LoraModule/mac/region/Region.d ./Core/LoraModule/mac/region/Region.o ./Core/LoraModule/mac/region/RegionAS923.d ./Core/LoraModule/mac/region/RegionAS923.o ./Core/LoraModule/mac/region/RegionBaseUS.d ./Core/LoraModule/mac/region/RegionBaseUS.o ./Core/LoraModule/mac/region/RegionCN779.d ./Core/LoraModule/mac/region/RegionCN779.o ./Core/LoraModule/mac/region/RegionCommon.d ./Core/LoraModule/mac/region/RegionCommon.o ./Core/LoraModule/mac/region/RegionEU433.d ./Core/LoraModule/mac/region/RegionEU433.o ./Core/LoraModule/mac/region/RegionEU868.d ./Core/LoraModule/mac/region/RegionEU868.o ./Core/LoraModule/mac/region/RegionIN865.d ./Core/LoraModule/mac/region/RegionIN865.o ./Core/LoraModule/mac/region/RegionKR920.d ./Core/LoraModule/mac/region/RegionKR920.o ./Core/LoraModule/mac/region/RegionRU864.d ./Core/LoraModule/mac/region/RegionRU864.o

.PHONY: clean-Core-2f-LoraModule-2f-mac-2f-region

