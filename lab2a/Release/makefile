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
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
lab2a.lss \

FLASH_IMAGE += \
lab2a.hex \

EEPROM_IMAGE += \
lab2a.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: lab2a.elf secondary-outputs

# Tool invocations
lab2a.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,lab2a.map -Wl,-u,vfprintf -lprintf_flt -lm -L"/home/james/workspace_old/RBELib/Release" -mmcu=atmega644p -o "lab2a.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

lab2a.lss: lab2a.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S lab2a.elf  >"lab2a.lss"
	@echo 'Finished building: $@'
	@echo ' '

lab2a.hex: lab2a.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex lab2a.elf  "lab2a.hex"
	@echo 'Finished building: $@'
	@echo ' '

lab2a.eep: lab2a.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex lab2a.elf  "lab2a.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: lab2a.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega644p lab2a.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) lab2a.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
