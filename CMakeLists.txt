# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Definições específicas do Dilithium
add_compile_definitions("DILITHIUM_MODE=2")
add_compile_definitions("USE_SHAKE")
add_compile_definitions("USE_AES=0")
add_compile_definitions("USE_RANDOMBYTES=1")
add_compile_definitions("USE_PACKING=1")
add_compile_definitions("REDUCE_FAST=1")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(testedilithium)
