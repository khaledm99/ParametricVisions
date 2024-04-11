#pragma once

#include "Bspline.h"


class RevolutionSurface {
private:
	Bspline curve;
	CPU_Geometry_Index surfaceGeom;
	GPU_Geometry_Index gpuGeom;
public:
	RevolutionSurface(Bspline& curve);
	RevolutionSurface(const RevolutionSurface& other);
	RevolutionSurface& operator=(const RevolutionSurface& other);
	void build();
	void draw();
};