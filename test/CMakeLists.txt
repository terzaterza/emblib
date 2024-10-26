message(STATUS "Setting up tests")

enable_testing()

add_executable(tests
    algo/iir.test.cpp
    algo/ekf.test.cpp
    algo/kalman.test.cpp
    algo/pid.test.cpp
    common/log.test.cpp
    drivers/accelerometer.test.cpp
    drivers/gyroscope.test.cpp
    math/custom/matrix_view.test.cpp
    math/custom/matrix.test.cpp
    math/matrix.test.cpp
    math/vector.test.cpp
    rtos/task.test.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain emblib)

include(CTest)
include(Catch)
catch_discover_tests(tests)