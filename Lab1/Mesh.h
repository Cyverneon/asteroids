#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"
#include <iostream>
#include <vector>

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;

	glm::vec3 tangent;
	glm::vec3 bitantent;
};

class Mesh {
public:
	Mesh();
	~Mesh();

	void draw();
	void init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void initModel(const IndexedModel& model);
	void loadModel(const std::string& filename);

private:
	GLuint vao = 0;  // Vertex Array Object
	GLuint vbo = 0;  // Interleaved Vertex Buffer Object
	GLuint ebo = 0;  // Index Buffer Object
	unsigned int drawCount = 0; // Number of indices
};
