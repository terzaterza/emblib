cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
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
add_subdirectory("lib/eigen")
add_subdirectory("lib/Catch2")
add_subdirectory("lib/FreeRTOS-Kernel")
add_subdirectory("test")


add_library(emblib STATIC
    src/common/log.cpp
    src/drivers/serial_device.cpp
    src/rtos/freertos/task.cpp
)

target_include_directories(emblib PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(emblib PUBLIC
    freertos_kernel
    etl::etl
    glm::glm-header-only
    eigen
)

# TODO: Add a custom command for testing (cmake test)