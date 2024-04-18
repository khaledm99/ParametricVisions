#pragma once

#include "Bspline.h"


class RevolutionSurface {
private:
	CPU_Geometry_Index surfaceGeom;
	GPU_Geometry_Index gpuGeom;
public:
    RevolutionSurface(){};
	RevolutionSurface(Bspline& curve);
	RevolutionSurface(const RevolutionSurface& other);
	RevolutionSurface& operator=(const RevolutionSurface& other);
	Bspline curve;
	void build();
	void draw();
    void drawCurves();
};
