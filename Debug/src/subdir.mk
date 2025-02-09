################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Driver.cpp \
../src/lexor.cpp \
../src/parser.cpp 

CPP_DEPS += \
./src/Driver.d \
./src/lexor.d \
./src/parser.d 

OBJS += \
./src/Driver.o \
./src/lexor.o \
./src/parser.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Driver.d ./src/Driver.o ./src/lexor.d ./src/lexor.o ./src/parser.d ./src/parser.o

.PHONY: clean-src

