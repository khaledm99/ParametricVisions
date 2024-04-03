#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "object.h"

Object::Object()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}

std::vector<float> const Object::getVertices()
{
    return vertices;
}

void Object::setVertices(std::vector<float> &v)
{
    vertices = v;
}

void Object::addVertex(glm::vec3 v)
{
    vertices.push_back(v.x);
    vertices.push_back(v.y);
    vertices.push_back(v.z);
}
void Object::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.data()), vertices.data(), GL_STATIC_DRAW);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);

}
