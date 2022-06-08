################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/sequencer/subdir.mk
-include Utilities/lpm/tiny_lpm/subdir.mk
-include STM32_WPAN/Target/subdir.mk
-include STM32_WPAN/App/subdir.mk
-include Middlewares/ST/STM32_WPAN/utilities/subdir.mk
-include Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/subdir.mk
-include Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/svc/Src/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/core/template/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/core/auto/subdir.mk
-include Drivers/STM32WBxx_HAL_Driver/Src/subdir.mk
-include Application/User/Startup/subdir.mk
-include Application/User/Core/subdir.mk
-include Application/User/App/subdir.mk
-include Application/LoraModule/system/subdir.mk
-include Application/LoraModule/radio/wifi/subdir.mk
-include Application/LoraModule/radio/lr1110_driver/subdir.mk
-include Application/LoraModule/radio/gnss/subdir.mk
-include Application/LoraModule/radio/subdir.mk
-include Application/LoraModule/peripherals/lr1110-se/subdir.mk
-include Application/LoraModule/mac/region/subdir.mk
-include Application/LoraModule/mac/subdir.mk
-include Application/LoraModule/common/LmHandler/packages/subdir.mk
-include Application/LoraModule/common/LmHandler/subdir.mk
-include Application/LoraModule/common/subdir.mk
-include Application/LoraModule/boards/subdir.mk
-include Application/LoraModule/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := LoraAssistTracker
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
LoraAssistTracker.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
LoraAssistTracker.list \

OBJCOPY_HEX += \
LoraAssistTracker.hex \


# All Target
all: main-build

# Main-build Target
main-build: LoraAssistTracker.elf secondary-outputs

# Tool invocations
LoraAssistTracker.elf: $(OBJS) $(USER_OBJS) D:\LoraEdge\Code\Git\LoraAssetMoniter\LoraAssetTracker\Projects\LoraAssetTracker\STM32WB15CCUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "LoraAssistTracker.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\LoraEdge\Code\Git\LoraAssetMoniter\LoraAssetTracker\Projects\LoraAssetTracker\STM32WB15CCUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="LoraAssistTracker.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

LoraAssistTracker.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "LoraAssistTracker.list"
	@echo 'Finished building: $@'
	@echo ' '

LoraAssistTracker.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "LoraAssistTracker.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) LoraAssistTracker.elf LoraAssistTracker.hex LoraAssistTracker.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
