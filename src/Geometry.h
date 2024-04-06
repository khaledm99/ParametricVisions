#pragma once

//------------------------------------------------------------------------------
// This file contains simple classes for storing geomtery on the CPU and the GPU
// Later assignments will require you to expand these classes or create your own
// similar classes with the needed functionality
//------------------------------------------------------------------------------

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>


// List of vertices and colour using std::vector and glm::vec3
struct CPU_Geometry {
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> cols;
};


// VAO and two VBOs for storing vertices and colours, respectively
class GPU_Geometry {

public:
	GPU_Geometry();

	// Public interface
	void bind() { vao.bind(); }

	void setVerts(const std::vector<glm::vec3>& verts);
	void setCols(const std::vector<glm::vec3>& cols);

private:
	// note: due to how OpenGL works, vao needs to be 
	// defined and initialized before the vertex buffers
	VertexArray vao;

	VertexBuffer vertBuffer;
	VertexBuffer colBuffer;
};

// VAO and two VBOs for storing vertices and colours, respectively
class GPU_Geometry_Index {

public:
	GPU_Geometry_Index();

	// Public interface
	void bind() { vao.bind(); ibo.bind(); }

	void setVerts(const std::vector<glm::vec3>& verts);
	void setCols(const std::vector<glm::vec3>& cols);
	void setIndices(const std::vector<unsigned int>& inds);

private:
	// note: due to how OpenGL works, vao needs to be 
	// defined and initialized before the vertex buffers
	VertexArray vao;

	IndexBuffer ibo;

	VertexBuffer vertBuffer;
	VertexBuffer colBuffer;
};
