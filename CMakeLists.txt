# Minimum CMake version required
cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

# Add a message for debug purposes and to avoid cmake warning
MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# Project Definition
project(SampleProject
        VERSION 0.0.1
        DESCRIPTION "A sample project for the STM32H750xB controller"
        HOMEPAGE_URL "https://google.com" # TODO: Add URL here
        LANGUAGES CXX C)

# Output file definition
set(EXECUTABLE ${PROJECT_NAME}.out)

# BSP Generated Files
set(STM32CUBEMX_GENERATED_FILES

        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fdcan.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_adc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
        BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c

        BSP/Src/main.c
        BSP/Src/stm32h7xx_hal_msp.c
        BSP/Src/stm32h7xx_it.c
        BSP/Src/system_stm32h7xx.c

        BSP/startup_stm32h750xx.s
        )

# Executable Definition
add_executable(${EXECUTABLE}
        ${STM32CUBEMX_GENERATED_FILES}
        src/Source/app.cpp
        src/Source/tx_initialize_low_level.S
        )

# ThreadX
set(THREADX_ARCH "cortex_m7")
target_link_libraries(${EXECUTABLE} PRIVATE threadx)
add_subdirectory(threadx SYSTEM)

# Languages
enable_language(CXX C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_compile_features(${EXECUTABLE} PRIVATE cxx_std_${CMAKE_CXX_STANDARD})

# BSP Definitions
target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32H750xx
        )

# Include Directories
target_include_directories(${EXECUTABLE} SYSTEM PRIVATE
        BSP/Inc
        BSP/Drivers/STM32H7xx_HAL_Driver/Inc
        BSP/Drivers/CMSIS/Device/ST/STM32H7xx/Include
        BSP/Drivers/CMSIS/Include

        src/Include
        )

# Compiler Flags
set(COMPILER_PROCESSOR_FLAGS
        -mcpu=cortex-m7
        -mthumb
        -mfloat-abi=hard
        -mfpu=fpv5-d16
        -mlong-calls
        )
set(LINKER_PROCESSOR_FLAGS ${COMPILE_PROCESSOR_FLAGS})

set(COMPILER_FEATURE_FLAGS
        -fdata-sections
        -ffunction-sections
        -fno-exceptions
        $<$<COMPILE_LANGUAGE:CXX>: -fno-rtti>
        )
set(LINKER_FEATURE_FLAGS ${COMPILE_FEATURE_FLAGS})

set(COMPILER_SPECS_FLAGS --specs=nosys.specs)
set(LINKER_SPECS_FLAGS ${COMPILER_SPECS_FLAGS})

set(COMPILER_WARNING_FLAGS
        -Wall
        -Wextra
        -Werror
        -pedantic

        $<$<COMPILE_LANGUAGE:CXX>:
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override
        -Wnon-virtual-dtor>
        )
set(LINKER_WARNING_FLAGS)

set(COMPILER_OPTIMIZATION_FLAGS -Os)
set(LINKER_OPTIMIZATION_FLAGS -Oz)

set(COMPILER_EXTRA_OPTIONS $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp> )
set(LINKER_EXTRA_OPTIONS -Wl,-Map=${PROJECT_NAME}.map,--cref,--print-memory-usage,--gc-sections)

#if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
target_compile_options(${EXECUTABLE} PRIVATE
        ${COMPILER_PROCESSOR_FLAGS}
        ${COMPILER_FEATURE_FLAGS}
        ${COMPILER_SPECS_FLAGS}
        ${COMPILER_WARNING_FLAGS}
        ${COMPILER_OPTIMIZATION_FLAGS}
        ${COMPILER_EXTRA_OPTIONS}
        )

# Linker Flags
target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/BSP/STM32H750XBHx_FLASH.ld
        ${LINKER_PROCESSOR_FLAGS}
        ${LINKER_FEATURE_FLAGS}
        ${LINKER_SPECS_FLAGS}
        ${LINKER_WARNING_FLAGS}
        ${LINKER_OPTIMIZATION_FLAGS}
        ${LINKER_EXTRA_OPTIONS}
        )

# Specific Warning Ignores
set_source_files_properties(BSP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_adc.c
        PROPERTIES
        COMPILE_FLAGS -Wno-pedantic)

set_source_files_properties(BSP/Src/stm32h7xx_hal_msp.c
        PROPERTIES
        COMPILE_FLAGS -Wno-unused-parameter)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${SIZE} ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)