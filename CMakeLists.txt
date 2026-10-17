cmake_minimum_required(VERSION 3.20)

project(c_audio_lab C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(wav STATIC audio/wav.c)

add_executable(wav_generator examples/wav_generator.c)
target_link_libraries(wav_generator PRIVATE wav m)
