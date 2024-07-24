# emblibcpp

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

## CMake configuration

### Emblibcpp
Configuration file should be provided by creating `emblib_config` target with the include directory attached providing the `emblib_config.hpp`.


### FreeRTOS
FreeRTOS port will be provided by default, depending on the host operating system (GCC_POSIX on Linux). This can be overriden by specifying the port by setting the `FREERTOS_PORT` in a parent CMake project.

For the configuration, `freertos_config` target can be provided with the include directory attached providing the `FreeRTOSConfig.h` file. If one does not exist, a default config file will be used, based on the GCC_POSIX port.

FreeRTOS (static) library can also be provided, be creating `freertos_kernel` target in a parent CMake project.