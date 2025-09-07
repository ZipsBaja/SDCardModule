cmake_minimum_required(VERSION 3.5)

set(PICO_BOARD pico)
#set(PICO_BOARD pico_w)
#set(PICO_BOARD pico2)

set(PICO_PLATFORM rp2040)
# set(PICO_PLATFORM rp2350-arm-s)

add_compile_definitions(USING_PICO)

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

set(C_STANDARD 17)
set(CXX_STANDARD 17)

pico_sdk_init()

project(Example C CXX ASM)

add_subdirectory(../ build)

add_executable(Example src/main.cpp)

target_link_libraries(Example
    ZipsSDCardModule
    pico_stdlib
)

pico_add_extra_outputs(Example)