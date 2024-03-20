#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Built from OpenGl tutorial at: https://learnopengl.com/Getting-started/Shaders
class Shader
{
    public:
        unsigned int id;

        Shader(const char* vertPath, const char* fragPath);

        void use();

        void setBool(const std::string &name, bool val) const;
        void setInt(const std::string &name, int val) const;
        void setFloat(const std::string &name, float val) const;
};


