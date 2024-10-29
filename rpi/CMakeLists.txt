# Project Setup
cmake_minimum_required(VERSION 3.24)
project(
  projet
  VERSION 1.0.0
  DESCRIPTION "WAL Raspberry Pi 433 MHz driver"
  LANGUAGES CXX C
)
set(TARGET wal_server_rpi)

# Source files
set(SOURCES "${PROJECT_SOURCE_DIR}/src")
add_executable(${TARGET}
    ${SOURCES}/uart.cpp
    ${SOURCES}/main.cpp
)

# Setting target properties for output directories
set_target_properties(${TARGET}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Including the include folder
target_include_directories(${TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Compilation
target_compile_options(${TARGET} PUBLIC -Wall -Wextra -Wpedantic -Werror)
