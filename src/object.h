#ifndef OBJECT_H
#define OBJECT_H

#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


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
	// VAO object, VBO shader
	int id;
	GLuint VAO, VBO;

	std::vector<glm::vec3> vt_positions;
	std::vector<glm::vec2> vt_textures;
	std::vector<glm::vec3> vt_normals;
	std::vector<Vertex> vertices;

	int numVertices;

	glm::mat4 model = glm::mat4(1.0);

	Object();

	Object(const char* path, glm::vec3 obj_pos, float obj_scale, int identifier);
	
	void MakeObject(GLuint shaderID, bool shader_texture, bool shader_normal);

	void draw();

	void print();
};
#endif