#pragma once

#include "emblib.hpp"
#include <chrono>

namespace emblib::time {

#if EMBLIB_TIME_ALIGN_MILLIS

typedef std::chrono::duration<int32_t, std::micro>      nanosec;
typedef std::chrono::duration<int32_t, std::milli>      microsec;
typedef std::chrono::duration<int32_t>                  millisec;
typedef std::chrono::duration<int32_t, std::kilo>       sec;

#else

typedef std::chrono::duration<int32_t, std::nano>       nanosec;
typedef std::chrono::duration<int32_t, std::micro>      microsec;
typedef std::chrono::duration<int32_t, std::milli>      millisec;
typedef std::chrono::duration<int32_t>                  sec;

#endif


#if EMBLIB_TIME_TICK_MILLIS

typedef millisec tick;

#else

#error "Implement alternative to millisec ticks"

#endif

}