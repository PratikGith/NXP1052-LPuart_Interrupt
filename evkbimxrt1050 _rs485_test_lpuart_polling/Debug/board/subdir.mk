################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/dcd.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/dcd.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/dcd.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1052DVL6B -DCPU_MIMXRT1052DVL6B_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\board" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\source" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\utilities" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\drivers" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\device" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\component\uart" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\component\lists" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\xip" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\CMSIS" -I"C:\Users\digi-pc-1\Documents\MCUXpressoIDE_11.5.0_7232\workspace\evkbimxrt1050 _rs485_test_lpuart_polling\evkbimxrt1050\driver_examples\lpuart\polling" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


