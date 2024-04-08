#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"

class FreeformSurface {
public:
	FreeformSurface(std::vector<std::vector<glm::vec3>> &controlPoints, int u_k, int v_k);
	int build();
	void draw();
private:
	// control points must be  a (u_m +1) x (v_m + 1) matrix
	std::vector<std::vector<glm::vec3>> controlPoints; // large vector is for u curve, inner vector is for v curve
	std::vector<int> uKnotSequence;
	std::vector<int> vKnotSequence;
	int u_k; //order of u curve
	int v_k; //order of v curve
	int u_m;
	int v_m;

	CPU_Geometry_Index surfaceGeom;

	int delta(float u, int k, int m, std::vector<int>& knotSequence);

	glm::vec3 E_delta_2(float u, float v);

	std::vector<int> computeStandardKnotSequence(int k, int m);
};