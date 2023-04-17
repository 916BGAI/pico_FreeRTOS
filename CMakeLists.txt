# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/kjlsai/Code/Pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(FreeRTOS C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
aux_source_directory(Src SRCS)

add_link_options(-Wl,--no-warn-rwx-segments)
add_executable(${PROJECT_NAME} ${SRCS})

pico_set_program_name(${PROJECT_NAME} "FreeRTOS")
pico_set_program_version(${PROJECT_NAME} "0.1")

# no_flash means the target is to run from RAM
# pico_set_binary_type(FreeRTOS no_flash)

pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)

# Add the standard library to the build
target_link_libraries(${PROJECT_NAME}
        pico_stdlib
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_multicore
        )

MACRO(HEADER_DIRECTORIES return_list)
  FILE(GLOB_RECURSE new_list
                    Inc/*.h
                    Inc/*.hpp
                    )
  SET(dir_list"")
  FOREACH(file_path ${new_list})
    GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
    SET(dir_list ${dir_list} ${dir_path})
  ENDFOREACH()
  LIST(REMOVE_DUPLICATES dir_list)
  SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

# Add the standard include files to the build
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  ${header_dir_list}
  )

target_compile_definitions(FreeRTOS PRIVATE
  mainRUN_FREE_RTOS_ON_CORE=1
  PICO_STACK_SIZE=0x1000
  PICO_STDIO_STACK_BUFFER_SIZE=64 # use a small printf on stack buffer
  )

# Add any user requested libraries
target_link_libraries(${PROJECT_NAME}
        # hardware_spi
        # hardware_i2c
        # hardware_dma
        # ardware_pio
        # hardware_interp
        # hardware_timer
        # hardware_watchdog
        # hardware_clocks
        )

pico_add_extra_outputs(${PROJECT_NAME})

string(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,--print-memory-usage")
