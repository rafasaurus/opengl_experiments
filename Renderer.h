#pragma once

#include <GL/glew.h>
#include <iostream>
#include <signal.h>

static void __debugbreak()
{
    std::cout << "[ASSERTION]" << std::endl;
    raise(SIGTRAP);
}

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);
