#pragma once

#include "Bspline.h"


class RevolutionSurface {
private:
	Bspline curve;
	CPU_Geometry_Index surfaceGeom;
	GPU_Geometry_Index gpuGeom;
public:
	RevolutionSurface(Bspline& curve);
	void build();
	void draw();
};