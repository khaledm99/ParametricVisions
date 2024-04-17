#pragma once

#include "Bspline.h"

class RuledSurface {
    public:
        Bspline curve_a;
        Bspline curve_b;
        CPU_Geometry_Index surfaceGeom;
        GPU_Geometry_Index gpuGeom;
    public:
        RuledSurface(){};
        RuledSurface(Bspline& curve_a, Bspline& curve_b);
        RuledSurface(const RuledSurface& other);
        RuledSurface& operator=(const RuledSurface& other);
        glm::vec3 surface(float u, float v);
        void build();
        void draw();
};
