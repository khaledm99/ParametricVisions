#pragma once

#include <glm/glm.hpp>


class Camera {
public:
    Camera();
    void reset();
    void update(glm::vec3 pos, float polar, float azimuth, float radius);
    void update(float polar, float azimuth);
    void update(float radius);
    glm::mat4 lookAt();
    
    float radius;
    float polar;
    float azimuth;
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
private:
    void update();
};
