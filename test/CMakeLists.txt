message(STATUS "Setting up tests")

enable_testing()

add_executable(tests
    algo/iir.test.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain emblib)

include(CTest)
include(Catch)
catch_discover_tests(tests)