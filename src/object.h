#pragma once

#include <glm/glm.hpp>
#include <vector>

class Object
{
    public:
        Object();
        //~Object();

        std::vector<float> const getVertices();
        void setVertices(std::vector<float> &v);
        void addVertex(glm::vec3 v);
        void draw();
    private:
        unsigned int vao;
        unsigned int vbo;
        std::vector<float> vertices;

};
