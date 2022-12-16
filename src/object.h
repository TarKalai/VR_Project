#ifndef OBJECT_H
#define OBJECT_H

#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

struct Vertex {
	glm::vec3 Position;
	glm::vec2 Texture;
	glm::vec3 Normal;
};

class Object
{
public:
	glm::vec3 position;
	float scale;
	int id;
	// VAO object, VBO shader
	GLuint VAO, VBO;

	std::vector<glm::vec3> vt_positions;
	std::vector<glm::vec2> vt_textures;
	std::vector<glm::vec3> vt_normals;
	std::vector<Vertex> vertices;

	int numVertices;

	glm::mat4 model = glm::mat4(1.0);

	Object(const char* path, glm::vec3 p, float s, int id);

	void MakeObject(GLuint shaderID, bool shader_texture, bool shader_normal);

	void draw();

	void print();
};
#endif