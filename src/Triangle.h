#pragma once

#include"Geometry.h"
#include <glm/glm.hpp>


class Triangle {
public:
	Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	void draw();

private:
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
};
