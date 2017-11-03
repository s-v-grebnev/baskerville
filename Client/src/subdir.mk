################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/basket.grpc.pb.cc \
../src/basket.pb.cc 

CPP_SRCS += \
../src/base64.cpp \
../src/client.cpp \
../src/coptions.cpp \
../src/rsa.cpp \
../src/serverfileop.cpp \
../src/soptions.cpp 

CC_DEPS += \
./src/basket.grpc.pb.d \
./src/basket.pb.d 

OBJS += \
./src/base64.o \
./src/basket.grpc.pb.o \
./src/basket.pb.o \
./src/client.o \
./src/coptions.o \
./src/rsa.o \
./src/serverfileop.o \
./src/soptions.o 

CPP_DEPS += \
./src/base64.d \
./src/client.d \
./src/coptions.d \
./src/rsa.d \
./src/serverfileop.d \
./src/soptions.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I/usr/local/include -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I/usr/local/include -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


