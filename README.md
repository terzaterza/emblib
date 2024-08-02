# emblib

C++ wrapper library for embedded development.

Intended to provide a unified interface to different underlying library implementations with common functionality. Allows writing platform independent code, and providing hardware specific drivers on board bring-up. Only static allocation used. Library dependencies can be customized or auto configured with default settings through CMake.

Some interfaces:
- RTOS
    - Mutex
    - Task (Thread)
    - Queue
- Math
    - Matrix
    - Vector
    - Quaternion
- Algorithms
    - Extended Kalman filter
    - IIR filter
    - PID controller
- Drivers
    - Generic serial devices
    - Accelerometer
    - Gyroscope

## Adding emblib to a project
As emblib depends on other libraries which are fetched as git submodules, the easiest way to include all of them is to clone this repository recursively into the project.
```shell
git clone --recursive https://github.com/terzaterza/emblibcpp.git
```
The library can then be included to a CMake project by adding this subdirectory, and linking the library to your target. CMake library (target) name is `emblib`.
```cmake
#Create emblib_config target here if needed

add_subdirectory("<path-to-cloned-repo>")

target_link_libraries(<project-target> PUBLIC/PRIVATE emblib)
```

## CMake configuration

### emblib
Configuration header file can (should) be provided by creating `emblib_config` target, before adding the emblibcpp subdirectory, with the include directory attached to the target providing the `emblib_config.hpp`.
```cmake
# EMBLIB configuration
add_library(emblib_config INTERFACE)

target_include_directories(emblib_config INTERFACE
    "<path-to-emblib_config.hpp>"
)
```
If this target is not provided, a default configuration is used.

### FreeRTOS
FreeRTOS port will be provided by default, depending on the host operating system (GCC_POSIX on Linux). This can be overriden by specifying the port by setting the `FREERTOS_PORT` in a parent CMake project.

For the configuration, `freertos_config` target can be provided with the include directory attached providing the `FreeRTOSConfig.h` file. If one does not exist, a default config file will be used, based on the GCC_POSIX port.

FreeRTOS (static) library can also be provided, be creating `freertos_kernel` target in a parent CMake project.