#include"Triangle.h"
#include <glad/glad.h>
#include <vector>


Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

void Triangle::draw() {
	GPU_Geometry_Index gpu_geom;
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> col;
	std::vector<unsigned int> ind;

	pos.push_back(p1);
	pos.push_back(p2);
	pos.push_back(p3);

	col.push_back(glm::vec3(1.0f, 0.f, 0.f));
	col.push_back(glm::vec3(1.0f, 0.f, 0.f));
	col.push_back(glm::vec3(1.0f, 0.f, 0.f));

	ind.push_back(0);
	ind.push_back(1);
	ind.push_back(2);

	gpu_geom.setVerts(pos);
	gpu_geom.setCols(col);
	gpu_geom.setIndices(ind);

	gpu_geom.bind();
	glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);

}
