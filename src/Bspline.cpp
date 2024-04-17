#include "Bspline.h"
#include <iostream>
#include <glad/glad.h>


Bspline::Bspline(std::vector<glm::vec3> controlPoints, int k) {
	this->controlPoints = controlPoints;
	this->k = k;
	this->knotSequence = computeStandardKnotSequence(this->k, this->controlPoints.size() - 1);
}

int Bspline::build() {
	if (this->controlPoints.size() < this->k) {
		return -1;
	}
	this->knotSequence = computeStandardKnotSequence(this->k, this->controlPoints.size() - 1);
	bsplineGeom.verts.clear();
	bsplineGeom.cols.clear();
	float color[3] = { 1.f, 0.f, 0.f }; // Color of new points

	//int range = knotSequence.back();
    int range = this->controlPoints.size();
	float increment = 0.01;
	//for (float u = knotSequence.at((this->k)-1); u <= knotSequence.at(range); u += increment) {
	for (float u = 0.f; u <= 1.f; u += increment) {
        bsplineGeom.verts.push_back(curve(u));
		bsplineGeom.cols.push_back(glm::vec3(color[0], color[1], color[2]));
	}
	gpuGeom.setVerts(bsplineGeom.verts);
	gpuGeom.setCols(bsplineGeom.cols);

	return 0;
}

glm::vec3 Bspline::curve(float u){
    if(this->controlPoints.size() < this->k) {
        return glm::vec3(0.f,0.f,0.f);
    }
    return E_delta_1(u, this->k, controlPoints.size()-1, knotSequence, controlPoints);
}

// bpsline needs to be built before drawn
void Bspline::draw() {
	gpuGeom.bind();
	glDrawArrays(GL_LINE_STRIP, 0, GLsizei(bsplineGeom.verts.size()));

}

int Bspline::delta(float u, int k, int m, std::vector<float>& knotSequence) {
	for (int i = 0; i < m + k; i++) {
		// if u is equal to the last and highest value of the knot, we are in trouble
		if (u >= knotSequence.at(i) && u < knotSequence.at(i + 1)) {
			return i;
		}
	}
	// std::cout << "invalid value for u = " << u << std::endl;
	return -1;
}

glm::vec3 Bspline::E_delta_1(float u, int k, int m, std::vector<float>& knotSequence, std::vector<glm::vec3>& controlPoints) {
	int d = delta(u, k, m, knotSequence);
	std::vector<glm::vec3> nonZeroControlPoints;
	for (int i = 0; i < k; i++) {
		// std::cout << " d - 1 = " << d - 1 << std::endl;
		nonZeroControlPoints.push_back(controlPoints.at(d - i));
	}
	for (int r = k; r >= 2; r--) {
		int i = d;
		for (int s = 0; s <= r - 2; s++) {
			float omega = (u - knotSequence.at(i)) / (knotSequence.at(i + r - 1) - knotSequence.at(i));
			nonZeroControlPoints.at(s) = omega * nonZeroControlPoints.at(s) + (1.f - omega) * nonZeroControlPoints[s + 1];
			i--;
		}
	}
	return nonZeroControlPoints.at(0);
}

std::vector<float> Bspline::computeStandardKnotSequence(int k, int m) {
	int size = m + k + 1;
	int padding = k * 2;
	int knots = size - padding;
	std::vector<float> U;
	for (int i = 0; i < k; i++) {
		U.push_back(0);
	}
	for (int j = 0; j < knots; j++) {
		//U.push_back(U.back() + 1);
        auto step = (j+1)*(1.f/(m-k+2));
        U.push_back(step);
	}
	//int finalPadding = U.back() + 1;
	for (int i = 0; i < k; i++) {
		U.push_back(1);
	}
    for(auto knot : U) 
	return U;
}

