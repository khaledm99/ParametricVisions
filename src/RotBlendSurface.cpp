#include "RotBlendSurface.h"
#include "Bspline.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <iostream>

RotationalBlendSurface::RotationalBlendSurface(std::vector<glm::vec3> controlPoints1, std::vector<glm::vec3> controlPoints2) {
	this->controlPoints1 = controlPoints1;
	this->controlPoints2 = controlPoints2;
	curve1 = Bspline(controlPoints1, 3);
	curve2 = Bspline(controlPoints2, 3);
}

RotationalBlendSurface::RotationalBlendSurface(Bspline &curve1, Bspline &curve2)
	:curve1(curve1), curve2(curve2)
{

}

int RotationalBlendSurface::build() {

	int rotationalIncAngle = 8;
	int totalRotation = 360;

	surfaceGeom.cols.clear();
	surfaceGeom.verts.clear();
	surfaceGeom.normals.clear();
	surfaceGeom.indices.clear();

	curve1.build();
	curve2.build();

	for (int i = 0; i < curve1.getGeom().verts.size(); i++) {
		const glm::vec3 &p1 = curve1.getGeom().verts.at(i);
		const glm::vec3 &p2 = curve2.getGeom().verts.at(i);

		glm::vec3 v = p2 - p1;
		glm::vec3 midpoint = (p1 + p2) * 0.5f;

		midline.verts.push_back(midpoint);
		midline.cols.push_back(glm::vec3(0.f, 0.f, 1.f));

		float angle = glm::atan(v.y / v.x);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -1.f * midpoint);

		glm::vec3 mc = glm::vec3(rotationMatrix * translationMatrix * glm::vec4(p1, 1.f));

		for (float i = 0; i <= maxAngle; i+=rotationalIncAngle) {
			glm::mat4 yRot = glm::rotate(glm::mat4(1.0f), glm::radians((float)i), glm::vec3(0.0f, 1.0f, 0.f));
			glm::vec4 nC = glm::inverse(translationMatrix) * glm::inverse(rotationMatrix)* yRot * glm::vec4(mc, 1.f);
			surfaceGeom.verts.push_back(glm::vec3(nC));
			surfaceGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));

		}
	}

	int layerSize = totalRotation / rotationalIncAngle + 1;

	for (int i = 0; i < surfaceGeom.verts.size(); i++) {
		if ((i % layerSize) - 1 >= 0 && (i + layerSize) < surfaceGeom.verts.size()) {

			unsigned int elements[] = { i, i - 1, i - 1 + layerSize, i - 1 + layerSize, i + layerSize, i };
			surfaceGeom.indices.insert(surfaceGeom.indices.end(), std::begin(elements), std::end(elements));

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




	return 0;
}


void RotationalBlendSurface::draw() {

	gpuGeom.setVerts(surfaceGeom.verts);
	gpuGeom.setCols(surfaceGeom.cols);
	gpuGeom.setIndices(surfaceGeom.indices);
	gpuGeom.setNormals(surfaceGeom.normals);

	gpuGeom.bind();
	glDrawElements(GL_TRIANGLES, surfaceGeom.indices.size(), GL_UNSIGNED_INT, 0);


}

void RotationalBlendSurface::drawMidline() {
	curve1.draw();
	curve2.draw();
	gpu_midline.setVerts(midline.verts);
	gpu_midline.setCols(midline.cols);
	gpu_midline.bind();
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)midline.verts.size());

}
void RotationalBlendSurface::drawCurves()
{
    curve1.draw();
    curve2.draw();
}
