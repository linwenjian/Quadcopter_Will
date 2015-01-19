################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Attitude_Control/Motor.obj: ../Attitude_Control/Motor.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/Attitude_Control" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/Filter" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/MPU6050" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/My_Source" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="Attitude_Control/Motor.pp" --obj_directory="Attitude_Control" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Attitude_Control/attitude_Control.obj: ../Attitude_Control/attitude_Control.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/Attitude_Control" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/Filter" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/MPU6050" --include_path="C:/Users/FGZ/workspace_v5_5/FlyBoard_Test/My_Source" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="Attitude_Control/attitude_Control.pp" --obj_directory="Attitude_Control" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


