#include "Line.h"
#include <glad/glad.h>
#include <vector>

Line::Line(glm::vec3 p1,glm::vec3 p2,glm::vec3 color)
{
    this->p1 = p1;
    this->p2 = p2;
    this->color = color;
}

void Line::draw()
{
	GPU_Geometry_Index gpu_geom;
	std::vector<glm::vec3> pos;
	std::vector<unsigned int> ind;
    std::vector<glm::vec3> col;

	pos.push_back(p1);
	pos.push_back(p2);

    col.push_back(color);

	ind.push_back(0);
	ind.push_back(1);

	gpu_geom.setVerts(pos);
	gpu_geom.setCols(col);
	gpu_geom.setIndices(ind);

	gpu_geom.bind();
	glDrawElements(GL_LINES, ind.size(), GL_UNSIGNED_INT, 0);

}
