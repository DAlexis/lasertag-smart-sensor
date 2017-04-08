cmake_minimum_required(VERSION 2.8)

project(smart-sensor C ASM)

set(EXE_SOURCES
	Drivers/newlib/newlib.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
	Src/rgb.c
	Src/system_stm32f0xx.c
	Src/ssp-driver.c
	Src/smart-sensor.c
	Src/gpio.c
	Src/tim.c
	Src/ir-receiver.c
	Src/main.c
	Src/stm32f0xx_hal_msp.c
	Src/usart.c
	Src/stm32f0xx_it.c
	Src/precision-clock.c
	lasertag-ssp/ssp/src/ssp-utils.c
	lasertag-ssp/ssp/src/ssp-slave.c
	lasertag-ssp/ssp/src/ssp-master.c
		
    Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_i2c_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_i2c.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_gpio.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_tim_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_uart_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_rcc_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_def.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_gpio_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_uart.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_flash_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_pwr_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dma.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_tim.h
	Drivers/STM32F0xx_HAL_Driver/Inc/Legacy/stm32_hal_legacy.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_pwr.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_flash.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dma_ex.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_rcc.h
	Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_cortex.h
	Drivers/CMSIS/Include/core_cm0plus.h
	Drivers/CMSIS/Include/core_cmInstr.h
	Drivers/CMSIS/Include/core_sc300.h
	Drivers/CMSIS/Include/arm_math.h
	Drivers/CMSIS/Include/arm_common_tables.h
	Drivers/CMSIS/Include/cmsis_armcc.h
	Drivers/CMSIS/Include/core_cm7.h
	Drivers/CMSIS/Include/cmsis_armcc_V6.h
	Drivers/CMSIS/Include/core_cmSimd.h
	Drivers/CMSIS/Include/core_cmFunc.h
	Drivers/CMSIS/Include/arm_const_structs.h
	Drivers/CMSIS/Include/core_sc000.h
	Drivers/CMSIS/Include/core_cm0.h
	Drivers/CMSIS/Include/core_cm3.h
	Drivers/CMSIS/Include/cmsis_gcc.h
	Drivers/CMSIS/Include/core_cm4.h
	Drivers/CMSIS/Device/ST/STM32F0xx/Include/system_stm32f0xx.h
	Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030x6.h
	Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f0xx.h
	Inc/main.h
	Inc/tim.h
	Inc/gpio.h
	Inc/usart.h
	Inc/stm32f0xx_hal_conf.h
	Inc/smart-sensor.h
	Inc/stm32f0xx_it.h
	Inc/ir-receiver.h
	Inc/rgb.h
	Inc/ir-timings.h
	Inc/precision-clock.h

    Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030x6.s
)

include_directories(
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32F0xx/Include
    Drivers/STM32F0xx_HAL_Driver/Inc
    Inc
    lasertag-ssp/ssp
    lasertag-ssp/ssp/Inc
)

set(ELF_NAME ${PROJECT_NAME}.elf)
set(BIN_NAME ${PROJECT_NAME}.bin)
set(MAP_NAME ${PROJECT_NAME}.map)


set(LDSCRIPTS "-T STM32F103RETx_FLASH.ld")
set(LDSCRIPTS_DIR "${PROJECT_SOURCE_DIR}/ldscripts")

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-g++)

# This two lines to avoid error 
# arm-none-eabi-g++: error: unrecognized command line option '-rdynamic'
# during compiler check. For some reasons test use '-rdynamic' by default, but arm-none-eabi-g* compilers does not support it
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# To remove -rdynamic
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(CMAKE_C_CXX_FLAGS "-mcpu=cortex-m0 -mthumb -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto  -Wall -Wextra")
set(CMAKE_C_CXX_FLAGS_DEBUG   "-fno-move-loop-invariants -DDEBUG -Os")
set(CMAKE_C_CXX_FLAGS_RELEASE "-Os")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_CXX_FLAGS} -std=gnu11")
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_C_CXX_FLAGS_DEBUG}" )
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CMAKE_C_CXX_FLAGS_RELEASE}" )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_C_CXX_FLAGS_DEBUG}" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_C_CXX_FLAGS_RELEASE}" )

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set(CMAKE_ASM_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}" )
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" )

add_definitions(-DSTM32F030x6)
#add_definitions(-DUSE_FULL_ASSERT)

set(LDSCRIPTS "-T STM32F030F4Px_FLASH.ld")
set(LDSCRIPTS_DIR "${PROJECT_SOURCE_DIR}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDSCRIPTS} -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Xlinker --gc-sections -L${LDSCRIPTS_DIR} -Wl,-Map,\"${MAP_NAME}\" --specs=nano.specs")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles")

add_executable(${ELF_NAME} ${EXE_SOURCES})

add_custom_command(TARGET ${ELF_NAME}
                   POST_BUILD
                   COMMAND arm-none-eabi-objcopy ARGS -O binary ${ELF_NAME}  ${BIN_NAME}
                   )

