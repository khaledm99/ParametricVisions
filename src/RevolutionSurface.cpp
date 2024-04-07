#include "RevolutionSurface.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <iostream>



RevolutionSurface::RevolutionSurface(Bspline &curve)
: curve(curve)
{
	
}

void RevolutionSurface::build() {
	CPU_Geometry curveGeom = curve.getGeom();

	surfaceGeom.cols.clear();
	surfaceGeom.verts.clear();
	surfaceGeom.indices.clear();

	for (int i = 0; i < 366; i+=2) {
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)i), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int j = 0; j < curveGeom.verts.size(); j++) {
			glm::vec3 vert = glm::vec3(rotationMatrix * glm::vec4(curveGeom.verts.at(j), 1.0f));
			glm::vec3 col = curveGeom.cols.at(j);
			surfaceGeom.verts.push_back(vert);
			surfaceGeom.cols.push_back(col);
		}

	}

	int height = curveGeom.verts.size();

	for (int i = 0; i < surfaceGeom.verts.size(); i++) {
		if ((i % height) - 1 >= 0 && (i + height) < surfaceGeom.verts.size()) {
			//unsigned int elements[] = { i, i - 1, i - 1 + height, i - 1 + height, i + height, i };
			//surfaceGeom.indices.insert(surfaceGeom.indices.end(), std::begin(elements), std::end(elements));
			surfaceGeom.indices.push_back(i);
			surfaceGeom.indices.push_back(i-1);
			surfaceGeom.indices.push_back(i-1+height);
			surfaceGeom.indices.push_back(i-1+height);
			surfaceGeom.indices.push_back(i+height);
			surfaceGeom.indices.push_back(i);

		}
	}

	std::cout << surfaceGeom.verts.size() << std::endl;

	//for (int i = 0; i < surfaceGeom.indices.size(); i++) {
	//	std::cout << surfaceGeom.indices.at(i) << std::endl;
	//}
}

void RevolutionSurface::draw() {
	GPU_Geometry_Index gpuGeom;
	gpuGeom.setVerts(surfaceGeom.verts);
	gpuGeom.setCols(surfaceGeom.cols);
	gpuGeom.setIndices(surfaceGeom.indices);

	gpuGeom.bind();

	//glDrawArrays(GL_POINTS, 0, (GLsizei)surfaceGeom.verts.size());
	glDrawElements(GL_TRIANGLES, surfaceGeom.indices.size(), GL_UNSIGNED_INT, 0);

	/*
	GPU_Geometry gpuGeom1;
	gpuGeom1.setVerts(surfaceGeom.verts);
	gpuGeom1.setCols(surfaceGeom.cols);
	gpuGeom1.bind();
	glDrawArrays(GL_POINTS, 0, (GLsizei)surfaceGeom.verts.size());
	*/


}
