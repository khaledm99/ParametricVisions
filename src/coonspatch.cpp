#include "bilinearpatch.h"
#include "Bspline.h"
#include "ruledsurface.h"
#include "coonspatch.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <iostream>
#include <cstdio>


CoonsPatch::CoonsPatch(Bspline p0,Bspline p1,Bspline q0,Bspline q1)
    : p0(p0)
    , p1(p1)
    , q0(q0)
    , q1(q1)
{}

CoonsPatch::CoonsPatch(const CoonsPatch& other)
    : p0(other.p0)
    , p1(other.p1)
    , q0(other.q0)
    , q1(other.q1)

{}

CoonsPatch& CoonsPatch::operator=(const CoonsPatch& other)
{
    if (this != &other){
         p0 = other.p0;
         p1=other.p1;
         q0=other.q0;
         q1=other.q1;
    }
    return *this;
}

void CoonsPatch::build()
{
	surfaceGeom.cols.clear();
	surfaceGeom.verts.clear();
	surfaceGeom.normals.clear();
	surfaceGeom.indices.clear();

    float scale = 1.f;
    for(auto& p : p0.controlPoints)
    {
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,scale));
        p = glm::vec3( translate * scaleM * glm::vec4(p,1.f));
    }

    for(auto& p : p1.controlPoints)
    {
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,-scale));
        p = glm::vec3( translate * scaleM * glm::vec4(p,1.f));
    }
    for(auto& p : q0.controlPoints)
    {
        auto rotate =  glm::rotate(glm::mat4(1.f),glm::radians(90.f), glm::vec3(0.f,1.f,0.f));
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(-scale,0.f,0.f));
        p = glm::vec3( translate * rotate * scaleM * glm::vec4(p,1.f));
    }
    for(auto& p : q1.controlPoints)
    {
        auto rotate =  glm::rotate(glm::mat4(1.f),glm::radians(90.f), glm::vec3(0.f,1.f,0.f));
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(scale,0.f,0.f));
        p = glm::vec3( translate * rotate * scaleM * glm::vec4(p,1.f));
    }


    if(glm::length(p0.controlPoints[0] - q0.controlPoints[0]) > 0.0001f) 
    {
        auto mid = (p0.controlPoints[0] + q0.controlPoints[0])/2.f;
        p0.controlPoints[0] = mid;
        q0.controlPoints[0] = mid;
    }

    if(glm::length(p0.controlPoints.back() - q1.controlPoints[0]) > 0.0001f) 
    {
        auto mid = (p0.controlPoints.back() + q1.controlPoints[0])/2.f;
        p0.controlPoints.back() = mid;
        q1.controlPoints[0] = mid;
    }

    if(glm::length(q0.controlPoints.back() - p1.controlPoints[0]) > 0.0001f) 
    {
        auto mid = (q0.controlPoints.back() + p1.controlPoints[0])/2.f;
        q0.controlPoints.back() = mid;
        p1.controlPoints[0] = mid;
    }

    if(glm::length(p1.controlPoints.back() - q1.controlPoints.back()) > 0.0001f) 
    {
        auto mid = (p1.controlPoints.back() + q1.controlPoints.back())/2.f;
        p1.controlPoints.back() = mid;
        q1.controlPoints.back() = mid;
    }

    p0.build();
    p1.build();
    q0.build();
    q1.build();
    s0 = RuledSurface(p0,p1);
    s1 = RuledSurface(q0,q1);
    s0.transform = false;
    s1.transform = false;
    s0.build();
    s1.build();
    p00 = p0.controlPoints[0];
    p01 = p0.controlPoints.back();
    p10 = q0.controlPoints.back();
    p11 = p1.controlPoints.back();
    bp = BilinearPatch(p00,p01,p10,p11);
    bp.build();

    
    float increment_u = 0.01f;
    float increment_v = 0.01f;
    int v_stride=0;
    for(float v=0.f;v<=1.f;v+=increment_v)
    {
        v_stride++;
    }

    float color[3] = {0.5f, 0.5f, 1.f};
    for (float u = 0.f; u<=1.f; u+= increment_u)
    {
        for(float v=0.f;v<=1.f; v+= increment_v)
        {
            auto r0 = s0.surface(u,v);
            auto r1 = s1.surface(v,u);
            auto b = bp.surface(u,v);
            auto mid = r0+r1;
            auto c = r0+r1-b;

            switch(step)
            {
                case 0:
                    surfaceGeom.verts.push_back(r0);
                    break;
                case 1:
                    surfaceGeom.verts.push_back(r1);
                    break;
                case 2:
                    surfaceGeom.verts.push_back(b);
                    break;
                case 3:
                    surfaceGeom.verts.push_back(mid);
                    break;
                case 4:
                    surfaceGeom.verts.push_back(c);
                    break;
            }
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
void CoonsPatch::draw() {
    
	GPU_Geometry_Index gpuGeom;
	gpuGeom.setVerts(surfaceGeom.verts);
	gpuGeom.setCols(surfaceGeom.cols);
	gpuGeom.setNormals(surfaceGeom.normals);
	gpuGeom.setIndices(surfaceGeom.indices);

	gpuGeom.bind();

	glDrawElements(GL_TRIANGLES, surfaceGeom.indices.size(), GL_UNSIGNED_INT, 0);
}
void CoonsPatch::drawCurves()
{
    p0.draw();
    p1.draw();
    q0.draw();
    q1.draw();
}
