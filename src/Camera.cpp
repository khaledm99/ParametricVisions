#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    yaw = pitch = 0.f;
    distance = -20.f;
    pos = glm::vec3(0.f,0.f,distance);
}
void Camera::reset()
{
    yaw = pitch = 0.f;
    distance = -20.f;
    pos = glm::vec3(0.f,0.f,distance);
    update();
}
void Camera::update(glm::vec3 pos, float yaw, float pitch, float distance)
{
    this->pos = pos;
    this->yaw = yaw;
    this->pitch = pitch;
    this->distance = distance;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(front,glm::vec3(0.f,1.f,0.f)));
        up = glm::normalize(glm::cross(right, front));
    this->pos *= distance;
}
void Camera::update(float yaw, float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
    update();
}
void Camera::update(float distance)
{
    this->distance = distance;
    this->pos *= distance;
    update();
}
void Camera::update()
{
    Camera::update(this->pos, this->yaw, this->pitch, this->distance);
}
glm::mat4 Camera::lookAt()
{
return glm::lookAt(this->pos, this->pos + this->front, this->up);
}
