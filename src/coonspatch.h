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

        RuledSurface s0;
        RuledSurface s1;

        BilinearPatch bp;

        CPU_Geometry_Index surfaceGeom;
        GPU_Geometry_Index gpuGeom;
        Bspline p0;
        Bspline p1;
        Bspline q0;
        Bspline q1;
        int step;
        CoonsPatch(){};
        CoonsPatch(Bspline p0,Bspline p1,Bspline q0,Bspline q1);
        CoonsPatch(const CoonsPatch& other);
        CoonsPatch& operator=(const CoonsPatch& other);
        glm::vec3 surface(float u, float v);
        void build();
        void draw();
        void drawCurves();
    private:
};
