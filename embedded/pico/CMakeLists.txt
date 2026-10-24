cmake_minimum_required(VERSION 3.17)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(pico_firmware C CXX ASM)

set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_executable(pico_firmware
    src/main.c
)

# SDK interface libraries add their sources to this target too.
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set_property(SOURCE src/main.c APPEND PROPERTY COMPILE_OPTIONS -Wall -Wextra)
endif()

target_link_libraries(pico_firmware PRIVATE
    pico_stdlib
    pico_audio_i2s
)

target_compile_definitions(pico_firmware PRIVATE
    PICO_AUDIO_I2S_DATA_PIN=9
    PICO_AUDIO_I2S_CLOCK_PIN_BASE=10
    PICO_AUDIO_I2S_MONO_INPUT=1
)

pico_add_extra_outputs(pico_firmware)
