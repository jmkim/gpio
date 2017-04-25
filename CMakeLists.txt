cmake_minimum_required(VERSION 3.7)
project(gpio)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES src/gpio.c src/gpio.h)
add_library(gpio SHARED ${SOURCE_FILES})