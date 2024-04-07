#include "FreeformSurface.h"
#include <glad/glad.h>
#include <iostream>

FreeformSurface::FreeformSurface(std::vector<std::vector<glm::vec3>> &controlPoints, int u_k, int v_k) {
	this->u_k = u_k;
	this->v_k = v_k;
	this->controlPoints = controlPoints;
	
	u_m = controlPoints.size() - 1;
	v_m = controlPoints.at(0).size() - 1;
}

void FreeformSurface::build() {
	// add error checking to make sure there are enough con
	uKnotSequence = computeStandardKnotSequence(u_k, u_m);
	vKnotSequence = computeStandardKnotSequence(v_k, v_m);

	surfaceGeom.cols.clear();
	surfaceGeom.verts.clear();
	surfaceGeom.indices.clear();

	float color[3] = { 1.f, 0.f, 0.f }; // Color of new points


	int range_v = vKnotSequence.back();
	float range_u = uKnotSequence.back();
	float increment_v = 0.01 * range_v;
	float increment_u = 0.01 * range_u;

	std::cout << range_u << " " << range_v << std::endl;

	for (float u = 0; u < range_u; u += increment_u) {
		for (float v = 0; v < range_v; v += increment_v) {
			surfaceGeom.verts.push_back(E_delta_2(u, v));
			surfaceGeom.cols.push_back(glm::vec3(color[0], color[1], color[2]));
		}
	}
	
}

void FreeformSurface::draw() {
	GPU_Geometry gpuGeom;
	gpuGeom.setVerts(surfaceGeom.verts);
	gpuGeom.setCols(surfaceGeom.cols);

	gpuGeom.bind();

	glDrawArrays(GL_POINTS, 0, (GLsizei)surfaceGeom.verts.size());
}

int FreeformSurface::delta(float u, int k, int m, std::vector<int>& knotSequence) {
	for (int i = 0; i < m + k; i++) {
		// if u is equal to the last and highest value of the knot, we are in trouble
		if (u >= knotSequence.at(i) && u < knotSequence.at(i + 1)) {
			return i;
		}
	}
	// std::cout << "invalid value for u = " << u << std::endl;
	return -1;
}

std::vector<int> FreeformSurface::computeStandardKnotSequence(int k, int m) {
	int size = m + k + 1;
	int padding = k * 2;
	int knots = size - padding;
	std::vector<int> U{};
	for (int i = 0; i < k; i++) {
		U.push_back(0);
	}
	for (int j = 0; j < knots; j++) {
		U.push_back(U.back() + 1);
	}
	int finalPadding = U.back() + 1;
	for (int i = 0; i < k; i++) {
		U.push_back(finalPadding);
	}
	return U;
}


glm::vec3 FreeformSurface::E_delta_2(float u, float v) {
	int u_d = delta(u, u_k, u_m, uKnotSequence);
	int v_d = delta(v, v_k, v_m, vKnotSequence);
	//std::cout << "got here "<< u_d <<" " << v_d << std::endl;

	std::vector<glm::vec3> C;
	for (int i = 0; i < u_k; i++) {
		std::vector<glm::vec3> D;
		for (int j = 0; j < v_k; j++) {
			//std::cout << "got here 1" << std::endl;
			D.push_back(controlPoints.at(u_d - i).at(v_d - j));
		}
		//std::cout << "got here 1" << std::endl;

		for (int r = v_k; r >= 2; r--) {
			int t = v_d;
			for (int s = 0; s <= r - 2; s++) {
				float omega = (v - vKnotSequence.at(t)) / (vKnotSequence.at(t + r - 1) - vKnotSequence.at(t));
				D.at(s) = omega * D.at(s) + (1.f - omega) * D.at(s + 1);
				t--;
			}
		}
		C.push_back(D.at(0));
		//return D.at(0);
	}
	for (int r = u_k; r >= 2; r--) {
		int t = u_d;
		for (int s = 0; s <= r - 2; s++) {
			float omega = (u - uKnotSequence.at(t)) / (uKnotSequence.at(t + r - 1) - uKnotSequence.at(t));
			C.at(s) = omega * C.at(s) + (1.f - omega) * C.at(s + 1);
			t--;
		}
	}
	//std::cout << "got here" << std::endl;
	return C.at(0);
}
