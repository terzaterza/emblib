cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(emblib VERSION 1.0)


# FreeRTOS
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE src)
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)


# GLM
set(GLM_BUILD_LIBRARY OFF)
set(GLM_BUILD_INSTALL OFF)
set(GLM_ENABLE_CXX_17 ON)


message(STATUS "GLM and ETL need to be built and installed, follow GitHub readmes for info")

add_subdirectory("lib/etl")
add_subdirectory("lib/glm")
add_subdirectory("lib/Catch2")
add_subdirectory("lib/FreeRTOS-Kernel")
add_subdirectory("test")


add_library(emblib INTERFACE
    src/common/log.cpp
    src/drivers/i2c_master.cpp
    src/drivers/serial_device.cpp
)

target_include_directories(emblib INTERFACE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(emblib INTERFACE
    freertos_kernel
    etl::etl
    glm::glm-header-only
)
