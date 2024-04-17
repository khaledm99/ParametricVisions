#include "bilinearpatch.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <iostream>

BilinearPatch::BilinearPatch(glm::vec3 p00,glm::vec3 p01,glm::vec3 p10,glm::vec3 p11)
    : p00(p00)
    , p01(p01)
    , p10(p10)
    , p11(p11)
{}

BilinearPatch::BilinearPatch(const BilinearPatch& other)
    : p00(other.p00)
    , p01(other.p01)
    , p10(other.p10)
    , p11(other.p11)
{}

BilinearPatch& BilinearPatch::operator=(const BilinearPatch& other)
{
    if (this != &other)
    { 
         p00=other.p00;
         p01=other.p01;
         p10=other.p10;
         p11=other.p11;
    }
    return *this;
}

void BilinearPatch::build()
{
	surfaceGeom.cols.clear();
	surfaceGeom.verts.clear();
	surfaceGeom.normals.clear();
	surfaceGeom.indices.clear();

    float increment_u = 0.01f;
    float increment_v = 0.01f;
    int v_stride=0;
    for(float v=0.f;v<=1.f;v+=increment_v)
    {
        v_stride++;
    }

    float color[3] = {0.5f, 0.5f, 1.f};
    for (float u = 0.f; u<1.f; u+= increment_u)
    {
        for(float v=0.f;v<1.f; v+= increment_v)
        {
            surfaceGeom.verts.push_back(surface(u,v));

			surfaceGeom.cols.push_back(glm::vec3(color[0], color[1], color[2]));

        }
    }
    for (int i = 0; i < surfaceGeom.verts.size(); i++) {
		if ((i % v_stride) - 1 >= 0 && (i + v_stride) < surfaceGeom.verts.size()) {
			
			unsigned int elements[] = { i, i - 1, i - 1 + v_stride, i - 1 + v_stride, i + v_stride, i };
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

	return;
}
glm::vec3 BilinearPatch::surface(float u, float v)
{                
    return (1-u)*(1-v)*p00 + u*(1-v)*p01 + (1-u)*v*p10 + u*v*p11;
}

void BilinearPatch::draw() {
	GPU_Geometry_Index gpuGeom;
	gpuGeom.setVerts(surfaceGeom.verts);
	gpuGeom.setCols(surfaceGeom.cols);
	gpuGeom.setNormals(surfaceGeom.normals);
	gpuGeom.setIndices(surfaceGeom.indices);

	gpuGeom.bind();

	//glDrawArrays(GL_POINTS, 0, (GLsizei)surfaceGeom.verts.size());
	glDrawElements(GL_TRIANGLES, surfaceGeom.indices.size(), GL_UNSIGNED_INT, 0);

}
