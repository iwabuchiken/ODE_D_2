################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../storage_ODE_D_2/main_D-2_v-1.1#5.cpp \
../storage_ODE_D_2/main_D-2_v-2.0#1.cpp \
../storage_ODE_D_2/main_D-2_v-2.0#2.cpp 

OBJS += \
./storage_ODE_D_2/main_D-2_v-1.1#5.o \
./storage_ODE_D_2/main_D-2_v-2.0#1.o \
./storage_ODE_D_2/main_D-2_v-2.0#2.o 

CPP_DEPS += \
./storage_ODE_D_2/main_D-2_v-1.1#5.d \
./storage_ODE_D_2/main_D-2_v-2.0#1.d \
./storage_ODE_D_2/main_D-2_v-2.0#2.d 


# Each subdirectory must supply rules for building sources it contributes
storage_ODE_D_2/%.o: ../storage_ODE_D_2/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DNULL=0 -I"C:\ode-0.11.1\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


