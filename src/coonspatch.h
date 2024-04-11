#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"
#include "Bspline.h"
#include "ruledsurface.h"
#include "bilinearpatch.h"

class CoonsPatch {
    public:
        glm::vec3 p00;
        glm::vec3 p01;
        glm::vec3 p10;
        glm::vec3 p11;

        /*
        BSpline q0;
        BSpline q1;
        BSpline p0;
        Bspline p1;
        */
        RuledSurface s0;
        RuledSurface s1;

        CPU_Geometry_Index surfaceGeom;
        GPU_Geometry_Index gpuGeom;
    public:
        CoonsPatch(glm::vec3 p00,glm::vec3 p01, glm::vec3 p10, glm::vec3 p11, RuledSurface s0, RuledSurface s1 );
        CoonsPatch(const CoonsPatch& other);
        CoonsPatch& operator=(const CoonsPatch& other);
        glm::vec3 surface(float u, float v);
        void build();
        void draw();
    private:
};
