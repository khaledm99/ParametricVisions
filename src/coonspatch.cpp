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

    float scale = 2.f;
    for(auto& p : p0.controlPoints)
    {
        //p = glm::vec3(glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(scale*2)), glm::vec3(0.f,0.f,scale)) * glm::vec4(p,1.f));
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,scale));
        p = glm::vec3( translate * scaleM * glm::vec4(p,1.f));
    }
    p0.build();

    for(auto& p : p1.controlPoints)
    {
        //p = glm::vec3(glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(scale*2)), glm::vec3(0.f,0.f,-scale)) * glm::vec4(p,1.f));
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,-scale));
        p = glm::vec3( translate * scaleM * glm::vec4(p,1.f));
    }
    p1.build();
    for(auto& p : q0.controlPoints)
    {
        //p = glm::vec3(glm::translate(glm::scale(glm::rotate(glm::mat4(1.f),glm::radians(90.f), glm::vec3(0.f,1.f,0.f)), glm::vec3(scale*2)), glm::vec3(0.f,0.f,-scale)) * glm::vec4(p,1.f));
        auto rotate =  glm::rotate(glm::mat4(1.f),glm::radians(90.f), glm::vec3(0.f,1.f,0.f));
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(-scale,0.f,0.f));
        p = glm::vec3( translate * rotate * scaleM * glm::vec4(p,1.f));
    }
    q0.build();
    for(auto& p : q1.controlPoints)
    {
        //p = glm::vec3(glm::translate(glm::scale(glm::rotate(glm::mat4(1.f),glm::radians(90.f), glm::vec3(0.f,1.f,0.f)), glm::vec3(scale*2)), glm::vec3(0.f,0.f,scale)) * glm::vec4(p,1.f));
        auto rotate =  glm::rotate(glm::mat4(1.f),glm::radians(90.f), glm::vec3(0.f,1.f,0.f));
        auto scaleM = glm::scale(glm::mat4(1.f), glm::vec3(scale));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(scale,0.f,0.f));
        p = glm::vec3( translate * rotate * scaleM * glm::vec4(p,1.f));
    }
    q1.build();


    s0 = RuledSurface(p0,p1);
    s1 = RuledSurface(q0,q1);
    s0.build();
    s1.build();
    
    /*
    printf("p0: %f,%f,%f\n", p0.controlPoints[0].x,p0.controlPoints[0].y,p0.controlPoints[0].z);
    printf("%f,%f,%f\n", p0.controlPoints.back().x,p0.controlPoints.back().y,p0.controlPoints.back().z);
    printf("p1: %f,%f,%f\n", p1.controlPoints[0].x,p1.controlPoints[0].y,p1.controlPoints[0].z);
    printf("%f,%f,%f\n", p1.controlPoints.back().x,p1.controlPoints.back().y,p1.controlPoints.back().z);
    printf("q0: %f,%f,%f\n", q0.controlPoints[0].x,q0.controlPoints[0].y,q0.controlPoints[0].z);
    printf("%f,%f,%f\n", q0.controlPoints.back().x,q0.controlPoints.back().y,q0.controlPoints.back().z);
    printf("q1: %f,%f,%f\n", q1.controlPoints[0].x,q1.controlPoints[0].y,q1.controlPoints[0].z);
    printf("%f,%f,%f\n", q1.controlPoints.back().x,q1.controlPoints[0].y,q1.controlPoints.back().z);
    */
    p00 = p0.controlPoints[0];
    //if(p00 != q0.controlPoints[0]) std::cout<<"error in p00"<<std::endl;
    //printf("%f,%f,%f\n", p0.controlPoints[0].x,p0.controlPoints[0].y,p0.controlPoints[0].z);
    //printf("%f,%f,%f\n", q0.controlPoints[0].x,q0.controlPoints[0].y,q0.controlPoints[0].z);
    p01 = p0.controlPoints.back();
    //if(p01 != q1.controlPoints[0]) std::cout<<"error in p01"<<std::endl;
    p10 = q0.controlPoints.back();
    //if(p10 != p1.controlPoints[0]) std::cout<<"error in p10"<<std::endl;
    p11 = p1.controlPoints.back();
    //if(p11 != q1.controlPoints.back()) std::cout<<"error in p11"<<std::endl;
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
            //surfaceGeom.verts.push_back(surface(u,v));
            auto r0 = s0.surface(u,v);
            auto r1 = s1.surface(v,u);
            auto b = bp.surface(u,v);
            auto c = r0+r1-b;
            if(false)
            {
            printf("r0: %f,%f,%f\n", r0.x,r0.y,r0.z);
            printf("r1: %f,%f,%f\n", r1.x,r1.y,r1.z);
            printf("b: %f,%f,%f\n", b.x,b.y,b.z);
            printf("c: %f,%f,%f\n", c.x,c.y,c.z);
            }

            surfaceGeom.verts.push_back(c);
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
    
    
    p0.draw();
    p1.draw();
    q0.draw();
    q1.draw();
    //bp.draw();

}
