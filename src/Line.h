#pragma once

#include "Geometry.h"
#include <glm/glm.hpp>

class Line {
    public:
        Line(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
        void draw();
    public:
        glm::vec3 p1;
        glm::vec3 p2;
        glm::vec3 color;
};
