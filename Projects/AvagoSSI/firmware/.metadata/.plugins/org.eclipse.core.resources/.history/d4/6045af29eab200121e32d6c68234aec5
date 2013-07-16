################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
AvagoSSI.hex \

SECONDARY_LIST += \
AvagoSSI.lst \

SECONDARY_SIZE += \
AvagoSSI.siz \


# All Target
all: AvagoSSI.elf secondary-outputs

# Tool invocations
AvagoSSI.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Summon Linux GCC C Linker'
	arm-none-eabi-gcc -nostartfiles -L/opt/libopencm3/arm-none-eabi/lib -Wl,-Map,AvagoSSI.map -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o "AvagoSSI.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

AvagoSSI.hex: AvagoSSI.elf
	@echo 'Invoking: ARM Summon Linux GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary AvagoSSI.elf  "AvagoSSI.hex"
	@echo 'Finished building: $@'
	@echo ' '

AvagoSSI.lst: AvagoSSI.elf
	@echo 'Invoking: ARM Summon Linux GNU Create Listing'
	arm-none-eabi-objdump -h -S AvagoSSI.elf > "AvagoSSI.lst"
	@echo 'Finished building: $@'
	@echo ' '

AvagoSSI.siz: AvagoSSI.elf
	@echo 'Invoking: ARM Summon Linux GNU Print Size'
	arm-none-eabi-size  --format=berkeley AvagoSSI.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) AvagoSSI.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
