#pragma once

#ifdef RTE_DEBUG
    #define RTE_ENABLE_ASSERTS
#endif

#ifdef RTE_ENABLE_ASSERTS
    #include "../Log.h"
    #define RTE_ASSERT(x, ...) { if(x){} else { RTE_FATAL("Assertion Failed: {0}", __VA_ARGS__); if(true){__debugbreak();} } }
    #define RTE_CORE_ASSERT(x, ...) { if(x){} else { RTE_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); if(true){ __debugbreak();} }}


    #define _ASSERT_GLUE(a, b)  a ## b
    #define ASSERT_GLUE(a, b)   _ASSERT_GLUE(a, b)
    #define STATIC_ASSERT(expr) \
            enum \
            { \
                ASSERT_GLUE(g_assert_fail_, __LINE__) = 1 / (int)(!!(expr)) \
            }
#else
    #define RTE_ASSERT(x, ...)
    #define RTE_CORE_ASSERT(x, ...)
    #define STATIC_ASSERT(expr)
#endif
