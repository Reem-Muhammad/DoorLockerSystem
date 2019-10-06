################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../I2C.c \
../LCD.c \
../Ocu.c \
../UART.c \
../hmi_ecu.c \
../hmi_ecu_functions.c \
../keypad.c \
../timer.c 

OBJS += \
./I2C.o \
./LCD.o \
./Ocu.o \
./UART.o \
./hmi_ecu.o \
./hmi_ecu_functions.o \
./keypad.o \
./timer.o 

C_DEPS += \
./I2C.d \
./LCD.d \
./Ocu.d \
./UART.d \
./hmi_ecu.d \
./hmi_ecu_functions.d \
./keypad.d \
./timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


