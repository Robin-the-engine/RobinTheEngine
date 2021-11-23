#pragma once

#ifdef RTE_PLATFORM_WINDOWS
    #ifdef RTE_DYNAMIC_LINK
        #ifdef RTE_BUILD_DLL
            #define RTE_API __declspec(dllexport)
        #else 
            #define RTE_API __declspec(dllimport)
        #endif // RTE_BUILD_DLL
    #else
        #define RTE_API
    #endif // RTE_DYNAMIC_LINK
#else
    #error Now only Windows support!
#endif // RTE_PLATFORM_WINDOWS
