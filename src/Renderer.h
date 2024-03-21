#pragma once

#include <glad/glad.h>
#include<assert.h>


#define ASSERT(x) if (!(x)) __debugbreak(); // this places a breakpoint and freezes the program where the error occurs. However you need to use Visual Studio
#define GLCall(x) GLClearError(); x; assert(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

