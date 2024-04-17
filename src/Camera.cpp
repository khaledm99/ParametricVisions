#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    polar = azimuth = 0.f;
    radius = -20.f;
    radius = -20.f;
    pos = glm::vec3(0.f,0.f,radius);
}
void Camera::reset()
{
    polar = azimuth = 0.f;
    radius = -20.f;
    radius = -20.f;
    pos = glm::vec3(0.f,0.f,radius);
    update();
}
void Camera::update(glm::vec3 pos, float polar, float azimuth, float radius)
{
    //  angle checking and arcball idea from https://www.mbsoftworks.sk/tutorials/opengl4/026-camera-pt3-orbit-camera/
    const auto fullCircle = 2.0f*glm::pi<float>();
    this->azimuth = fmodf(glm::radians(azimuth), fullCircle);
    if (this->azimuth < 0.0f) {
        this->azimuth = fullCircle + this->azimuth;
    }
    const auto polarCap = glm::pi<float>() / 2.0f - 0.001f;
    this->polar = glm::radians(polar);
    if (this->polar > polarCap) {
        this->polar = polarCap;
    }

    if (this->polar < -polarCap) {
        this->polar = -polarCap;
    }

    this->radius = radius;
    auto x = radius*glm::cos(this->polar)*glm::cos(this->azimuth);
    auto y = radius*glm::sin(this->polar);
    auto z = radius*glm::cos(this->polar)*glm::sin(this->azimuth);
    this->pos = glm::vec3(x,y,z);
    glm::vec3 direction;
    direction = glm::normalize(this->pos - glm::vec3(0.f));
    auto right = glm::normalize(glm::cross(direction, glm::vec3(0.f,1.f,0.f)));
    this->up = glm::normalize(glm::cross(right,direction));

    /*
    direction.x = cos(glm::radians(polar)) * cos(glm::radians(azimuth));
        direction.y = sin(glm::radians(azimuth));
        direction.z = sin(glm::radians(polar)) * cos(glm::radians(azimuth));
        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(front,glm::vec3(0.f,1.f,0.f)));
        up = glm::normalize(glm::cross(right, front));
        */
    //this->pos *= radius;
}
void Camera::update(float polar, float azimuth)
{
    this->polar = polar;
    this->azimuth = azimuth;
    update();
}
void Camera::update(float radius)
{
    this->radius = radius;
    this->pos *= radius;
    update();
}
void Camera::update()
{
    Camera::update(this->pos, this->polar, this->azimuth, this->radius);
}
glm::mat4 Camera::lookAt()
{
return glm::lookAt(this->pos, glm::vec3(0.f), this->up);
}
