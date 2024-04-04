#include "Geometry.h"

#include <utility>


GPU_Geometry::GPU_Geometry()
	: vao()
	, vertBuffer(0, 3, GL_FLOAT)
	, colBuffer(1, 3, GL_FLOAT)
{}


void GPU_Geometry::setVerts(const std::vector<glm::vec3>& verts) {
	vertBuffer.uploadData(sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
}


void GPU_Geometry::setCols(const std::vector<glm::vec3>& cols) {
	colBuffer.uploadData(sizeof(glm::vec3) * cols.size(), cols.data(), GL_STATIC_DRAW);
}


//-----------------------------------------------------------------------------------

GPU_Geometry_Index::GPU_Geometry_Index()
	: vao()
	, ibo()
	, vertBuffer(0, 3, GL_FLOAT)
	, colBuffer(1, 3, GL_FLOAT)
{}


void GPU_Geometry_Index::setVerts(const std::vector<glm::vec3>& verts) {
	vertBuffer.uploadData(sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
}


void GPU_Geometry_Index::setCols(const std::vector<glm::vec3>& cols) {
	colBuffer.uploadData(sizeof(glm::vec3) * cols.size(), cols.data(), GL_STATIC_DRAW);
}

void GPU_Geometry_Index::setIndices(const std::vector<unsigned int>& inds) {
	ibo.uploadData(sizeof(unsigned int) * inds.size(), inds.data(), GL_STATIC_DRAW);
}
