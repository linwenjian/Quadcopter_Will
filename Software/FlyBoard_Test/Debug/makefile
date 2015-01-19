################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/arm_5.1.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./Attitude_Control/attitude_Control.obj" \
"./Attitude_Control/Motor.obj" \
"./Filter/complementary_filter.obj" \
"./MPU6050/mpu6050dmp_attitude_angle_cal.obj" \
"./MPU6050/inv_mpu_dmp_motion_driver.obj" \
"./MPU6050/inv_mpu.obj" \
"./My_Source/spi.obj" \
"./My_Source/i2c.obj" \
"./My_Source/camera.obj" \
"./My_Source/SystemInit.obj" \
"./My_Source/OLED_DATA.obj" \
"./My_Source/OLED.obj" \
"./My_Source/NRF24L01.obj" \
"./My_Source/My_ext_Int.obj" \
"./My_Source/My_UART.obj" \
"./My_Source/My_PeriodTimer.obj" \
"./My_Source/My_PWM.obj" \
"./My_Source/My_GetTimes.obj" \
"./My_Source/My_DMA.obj" \
"./My_Source/My_ADC.obj" \
"./My_Source/LED.obj" \
"./My_Source/Keys.obj" \
"./My_Source/Delay.obj" \
"./uartstdio.obj" \
"./tm4c123gh6pm_startup_ccs.obj" \
"./main.obj" \
"../tm4c123gh6pm.cmd" \
-l"libc.a" \
-l"C:\ti\TivaWare_C_Series-2.1.0.12573\usblib\ccs\Debug\usblib.lib" \
-l"C:\ti\TivaWare_C_Series-2.1.0.12573\driverlib\ccs\Debug\driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include My_Source/subdir_vars.mk
-include MPU6050/subdir_vars.mk
-include Filter/subdir_vars.mk
-include Attitude_Control/subdir_vars.mk
-include subdir_rules.mk
-include My_Source/subdir_rules.mk
-include MPU6050/subdir_rules.mk
-include Filter/subdir_rules.mk
-include Attitude_Control/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: FlyBoard_Test.out

# Tool invocations
FlyBoard_Test.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --diag_warning=225 --display_error_number --diag_wrap=off -z -m"FlyBoard_Test.map" --stack_size=2048 -i"C:/ti/ccsv5/tools/compiler/arm_5.1.1/lib" -i"C:/ti/TivaWare_C_Series-2.1.0.12573" -i"C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="FlyBoard_Test_linkInfo.xml" --rom_model -o "FlyBoard_Test.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QUOTED) "FlyBoard_Test.out"
	-$(RM) "main.pp" "tm4c123gh6pm_startup_ccs.pp" "uartstdio.pp" "My_Source\Delay.pp" "My_Source\Keys.pp" "My_Source\LED.pp" "My_Source\My_ADC.pp" "My_Source\My_DMA.pp" "My_Source\My_GetTimes.pp" "My_Source\My_PWM.pp" "My_Source\My_PeriodTimer.pp" "My_Source\My_UART.pp" "My_Source\My_ext_Int.pp" "My_Source\NRF24L01.pp" "My_Source\OLED.pp" "My_Source\OLED_DATA.pp" "My_Source\SystemInit.pp" "My_Source\camera.pp" "My_Source\i2c.pp" "My_Source\spi.pp" "MPU6050\inv_mpu.pp" "MPU6050\inv_mpu_dmp_motion_driver.pp" "MPU6050\mpu6050dmp_attitude_angle_cal.pp" "Filter\complementary_filter.pp" "Attitude_Control\Motor.pp" "Attitude_Control\attitude_Control.pp" 
	-$(RM) "main.obj" "tm4c123gh6pm_startup_ccs.obj" "uartstdio.obj" "My_Source\Delay.obj" "My_Source\Keys.obj" "My_Source\LED.obj" "My_Source\My_ADC.obj" "My_Source\My_DMA.obj" "My_Source\My_GetTimes.obj" "My_Source\My_PWM.obj" "My_Source\My_PeriodTimer.obj" "My_Source\My_UART.obj" "My_Source\My_ext_Int.obj" "My_Source\NRF24L01.obj" "My_Source\OLED.obj" "My_Source\OLED_DATA.obj" "My_Source\SystemInit.obj" "My_Source\camera.obj" "My_Source\i2c.obj" "My_Source\spi.obj" "MPU6050\inv_mpu.obj" "MPU6050\inv_mpu_dmp_motion_driver.obj" "MPU6050\mpu6050dmp_attitude_angle_cal.obj" "Filter\complementary_filter.obj" "Attitude_Control\Motor.obj" "Attitude_Control\attitude_Control.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

