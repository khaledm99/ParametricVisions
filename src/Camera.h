#pragma once

#include <glm/glm.hpp>


class Camera {
public:
    Camera();
    void reset();
    void update(glm::vec3 pos, float yaw, float pitch, float distance);
    void update(float yaw, float pitch);
    void update(float distance);
    glm::mat4 lookAt();
    
    float yaw;
    float pitch; 
    float distance;
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
private:
    void update();
};
