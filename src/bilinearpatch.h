#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"

class BilinearPatch {
    public:
        glm::vec3 p00;
        glm::vec3 p01;
        glm::vec3 p10;
        glm::vec3 p11;
        CPU_Geometry_Index surfaceGeom;
        GPU_Geometry_Index gpuGeom;
    public:
        BilinearPatch(){};
        BilinearPatch(glm::vec3 p00,glm::vec3 p01, glm::vec3 p10, glm::vec3 p11 );
        BilinearPatch(const BilinearPatch& other);
        BilinearPatch& operator=(const BilinearPatch& other);
        glm::vec3 surface(float u, float v);
        void build();
        void draw();
};
