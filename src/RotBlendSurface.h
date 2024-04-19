#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"
#include "Bspline.h"

class RotationalBlendSurface {
public:
    RotationalBlendSurface(){};
	RotationalBlendSurface(std::vector<glm::vec3> controlPoints1, std::vector<glm::vec3> controlPoints2);
	RotationalBlendSurface(Bspline& curve1, Bspline& curve2);

	RotationalBlendSurface(const RotationalBlendSurface& other)
		: controlPoints1(other.controlPoints1), controlPoints2(other.controlPoints2), curve1(other.curve1), curve2(other.curve2) {
	}

	RotationalBlendSurface& operator=(const RotationalBlendSurface& other) {
		if (this != &other) {
			controlPoints1 = other.controlPoints1;
			controlPoints2 = other.controlPoints2;
			curve1 = other.curve1;
			curve2 = other.curve2;
			// Copy other members as needed...
		}
		return *this;
	}
	Bspline curve1;
	Bspline curve2;
	int build();
	void draw();
    void drawCurves();
	void drawMidline();
    float maxAngle = 360.f;

	std::vector<glm::vec3>& getControlPoints1() { return controlPoints1; }
	std::vector<glm::vec3>& getControlPoints2() { return controlPoints2; }
private:
	std::vector<glm::vec3> controlPoints1;
	std::vector<glm::vec3> controlPoints2;


	CPU_Geometry_Index surfaceGeom;
	GPU_Geometry_Index gpuGeom;

	CPU_Geometry midline;
	GPU_Geometry gpu_midline;


};
