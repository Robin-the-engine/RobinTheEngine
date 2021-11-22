#pragma once

#include "rtepch.h"

#ifdef RTE_PLATFORM_WINDOWS

#include <windows.h>

#ifndef RTE_BUILD_ENGINE

extern RTE::Application* RTE::CreateApplication();

int main(int argc, char** argv)
{
    RTE::Log::Init();

    auto app = RTE::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
#else
#include <iostream>

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    RTE::Log::Init();

    //std::cout << "Hello world\n";
    return 0;
}
#endif // RTE_BUILD_ENGINE
#endif // RTE_PLATFORM_WINDOWS
