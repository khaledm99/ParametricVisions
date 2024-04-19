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

RevolutionSurface::RevolutionSurface(const RevolutionSurface& other)
	: curve(other.curve) {
	// Copy other members as needed...
}

RevolutionSurface& RevolutionSurface::operator=(const RevolutionSurface& other) {
	if (this != &other) {
		// Copy curve
		curve = other.curve;

		// Copy other members as needed...
	}
	return *this;
}

void RevolutionSurface::build() {
    curve.build();
	CPU_Geometry curveGeom = curve.getGeom();

	surfaceGeom.cols.clear();
	surfaceGeom.verts.clear();
	surfaceGeom.normals.clear();
	surfaceGeom.indices.clear();

	for (float i = 0; i < maxAngle+6; i+=2) {
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
			surfaceGeom.indices.push_back(i);
			surfaceGeom.indices.push_back(i-1);
			surfaceGeom.indices.push_back(i-1+height);
			surfaceGeom.indices.push_back(i-1+height);
			surfaceGeom.indices.push_back(i+height);
			surfaceGeom.indices.push_back(i);

		}
	}

		
	surfaceGeom.normals = std::vector<glm::vec3>(surfaceGeom.verts.size(), glm::vec3(0.0f));

	for (size_t i = 0; i < surfaceGeom.indices.size(); i += 3) {
		glm::vec3 p1 = surfaceGeom.verts[surfaceGeom.indices[i]];
		glm::vec3 p2 = surfaceGeom.verts[surfaceGeom.indices[i + 1]];
		glm::vec3 p3 = surfaceGeom.verts[surfaceGeom.indices[i + 2]];

		glm::vec3 v1 = p2 - p1;
		glm::vec3 v2 = p3 - p2;
		glm::vec3 v3 = p1 - p3;

		surfaceGeom.normals[surfaceGeom.indices[i]] += glm::normalize(glm::cross(v1, v2)) * (float)acos(glm::dot(-v3, v1) / (glm::length(-v3) * glm::length(v1)));
		surfaceGeom.normals[surfaceGeom.indices[i + 1]] += glm::normalize(glm::cross(v1, v2)) * (float)acos(glm::dot(-v1, v2) / (glm::length(-v1) * glm::length(v2)));
		surfaceGeom.normals[surfaceGeom.indices[i + 2]] += glm::normalize(glm::cross(v1, v2)) * (float)acos(glm::dot(-v2, v3) / (glm::length(-v2) * glm::length(v3)));

	}

	for (auto& norms : surfaceGeom.normals) {
		norms = glm::normalize(norms);
	}
}

void RevolutionSurface::draw() {
	GPU_Geometry_Index gpuGeom;
	gpuGeom.setVerts(surfaceGeom.verts);
	gpuGeom.setCols(surfaceGeom.cols);
	gpuGeom.setNormals(surfaceGeom.normals);
	gpuGeom.setIndices(surfaceGeom.indices);

	gpuGeom.bind();

	glDrawElements(GL_TRIANGLES, surfaceGeom.indices.size(), GL_UNSIGNED_INT, 0);

}
void RevolutionSurface::drawCurves() {
    curve.draw();    
}
