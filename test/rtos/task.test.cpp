#include "emblib/rtos/task.hpp"
#include "emblib/common/time.hpp"
#include "catch2/catch_test_macros.hpp"

class test_task : public emblib::rtos::task<128> {

public:
    explicit test_task() noexcept :
        task("Test task", 1)
    {}

private:
    void run() noexcept override
    {
        while (1) {
            delay_until(emblib::time::sec(1));
        }
    }

};