#pragma once

#include "rtepch.h"

#ifdef RTE_PLATFORM_WINDOWS

#include <windows.h>

#ifndef RTE_BUILD_ENGINE
extern RTE::Application* RTE::CreateApplication();
#endif // RTE_BUILD_ENGINE

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    RTE::Log::Init();
#ifndef RTE_BUILD_ENGINE
    auto app = RTE::CreateApplication();
    app->Run();
    delete app;
#else
    OutputDebugStringA("Hello world\n");
#endif // RTE_BUILD_ENGINE
    return 0;
}

#endif // RTE_PLATFORM_WINDOWS
