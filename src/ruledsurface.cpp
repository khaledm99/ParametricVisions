#include "ruledsurface.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <iostream>

RuledSurface::RuledSurface(Bspline &curve_a, Bspline &curve_b)
    : curve_a(curve_a)
    , curve_b(curve_b)
{}

RuledSurface::RuledSurface(const RuledSurface& other)
    : curve_a(other.curve_a)
    , curve_b(other.curve_b)
{}

RuledSurface& RuledSurface::operator=(const RuledSurface& other)
{
    if (this != &other)
    {
        curve_a = other.curve_a;
        curve_b = other.curve_b;
    }
    return *this;
}

glm::vec3 RuledSurface::surface(float u, float v)
{
    return (1-v)*curve_a.curve(u) + v*curve_b.curve(u);
}
void RuledSurface::build()
{
	surfaceGeom.cols.clear();
	surfaceGeom.verts.clear();
	surfaceGeom.normals.clear();
	surfaceGeom.indices.clear();


    
    if(transform)
    {
        float scale = 1.f;
        for(auto& p : curve_a.controlPoints)
        {
            auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
            auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,scale));
            p = glm::vec3( translate * scaleM * glm::vec4(p,1.f));
        }

        for(auto& p : curve_b.controlPoints)
        {
            auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
            auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,-scale));
            p = glm::vec3( translate * scaleM * glm::vec4(p,1.f));
        }
    }
    



    curve_a.build();
    curve_b.build();
    CPU_Geometry curveGeomA = curve_a.getGeom();
    CPU_Geometry curveGeomB = curve_b.getGeom();
    /*
    if(curveGeomA.verts.size() != curveGeomB.verts.size())
    {
        std::cout<<"spline sizes don't match"<<std::endl;
        return;
    }
    */


    float color[3] = { 1.f, 0.f, 0.f }; // Color of new points


	//int range_u = curve_a.knotSequence.back();
    float range_u = curve_a.knotSequence[curve_a.controlPoints.size()];
	//float increment_u = 0.05 * range_u;
	float increment_u = 0.01f;

	float increment_v = 0.01f;
     //v_stride = 1.f/increment_v;
    int v_stride=0;
    for(float v=0.f;v<=1.f;v+=increment_v)
    {
        v_stride++;
    }


	bool doOnceU = true;
	bool doOnceV = true;
	
	//for (float u = curve_a.knotSequence[curve_a.k-1]; u <= range_u; u += increment_u) {
	for (float u = 0.f; u <= 1.f; u += increment_u) {
		for (float v = 0.f; v <= 1.f; v += increment_v) {
			//std::cout << "u: " << u << " v: " << v << std::endl;
            if (v + increment_v > 1.f && doOnceV == true){
                //u = range_u - 0.01 - increment_u;
                v = 1.f;
                doOnceV = false;
            }
			surfaceGeom.verts.push_back(surface(u,v));
			surfaceGeom.cols.push_back(glm::vec3(color[0], color[1], color[2]));
			
		}
		if (u + increment_u > 1.f && doOnceU == true){
			//u = range_u - 0.01 - increment_u;
            u = 1.f;
			doOnceU = false;
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
void RuledSurface::draw() {
	GPU_Geometry_Index gpuGeom;
	gpuGeom.setVerts(surfaceGeom.verts);
	gpuGeom.setCols(surfaceGeom.cols);
	gpuGeom.setNormals(surfaceGeom.normals);
	gpuGeom.setIndices(surfaceGeom.indices);

	gpuGeom.bind();

	//glDrawArrays(GL_POINTS, 0, (GLsizei)surfaceGeom.verts.size());
	glDrawElements(GL_TRIANGLES, surfaceGeom.indices.size(), GL_UNSIGNED_INT, 0);

}
void RuledSurface::drawCurves()
{
    curve_a.draw();
    curve_b.draw();
}
