#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"


class Bspline {
public:

	Bspline(){ }

	Bspline(std::vector<glm::vec3> controlPoints, int k);

	Bspline(const Bspline& other)
		:controlPoints(other.controlPoints),
		bsplineGeom(other.bsplineGeom),
		k(other.k)
	{

	}

	Bspline& operator=(const Bspline& other) {
		if (this != &other) {
			// Copy controlPoints
			controlPoints = other.controlPoints;

			// Copy bsplineGeom
			bsplineGeom = other.bsplineGeom;

			// Copy k
			k = other.k;

			// Copy other members as needed...
		}
		return *this;
	}

	//creates the geometry of the bspline and stores it into bsplineGeom
	int build();
    glm::vec3 curve(float u);

	//takes what is in the cpuGeom and draws it to the screen
	void draw();

    std::vector<float> knotSequence;
	const CPU_Geometry& getGeom() const { return bsplineGeom; }
	int k; //order of the bspline
	std::vector<glm::vec3> controlPoints;
private:

	CPU_Geometry bsplineGeom;

	GPU_Geometry gpuGeom;


	int delta(float u, int k, int m, std::vector<float>& knotSequence);
	
	glm::vec3 E_delta_1(float u, int k, int m, std::vector<float>& knotSequence, std::vector<glm::vec3>& controlPoints);

	std::vector<float> computeStandardKnotSequence(int k, int m);
};
