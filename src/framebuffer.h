#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Framebuffer 
{
    public:
        Framebuffer();
        void setup(int width, int height);
        void bind();
        void unbind();
        unsigned int const getFramebuffer();
        unsigned int const getTextureBuffer();
        unsigned int const getRenderbuffer();
    private:
        int width;
        int height;
        unsigned int fb;
        unsigned int texColBuf;
        unsigned int rbo;
};
