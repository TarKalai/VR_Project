#pragma once

#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "material.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec2 Texture;
	glm::vec3 Normal;
	glm::vec3 Tangent;
};

class Object
{
public:
	static int objectCounter;
	bool visible;
	bool bumpmap;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 color;
	// VAO object, VBO shader
	int id;
	GLuint VAO, VBO;
	const char* texturepath;

	GLuint u_texture;
	GLuint textureID;
	Texture* texture;
	Material* material;


	std::vector<glm::vec3> vt_positions;
	std::vector<glm::vec2> vt_textures;
	std::vector<glm::vec3> vt_normals;
	std::vector<Vertex> vertices;


	int numVertices;

	glm::mat4 model = glm::mat4(1.0);

	Object();
	
	Object(const char* geometryPath, Texture* tex, Material* matos,  glm::vec3 obj_pos, glm::vec3 obj_rot, glm::vec3 obj_scale, bool is_visible=true, glm::vec3 Color=glm::vec3(1.0), bool Bumpmap=false);
	
	void MakeObject();

	void setPosRot(glm::vec3 obj_pos, glm::vec3 obj_rot);
    void AssignPoNoTe(Vertex* vertex, std::string f);
	void AssignTangent(Vertex *v1, Vertex *v2, Vertex *v3);
	float * getData();
	void LoadVertices(const char* geometryPath);


	void draw();
	std::vector<glm::vec3> getVertexPosition();
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale(){return scale;}
	glm::vec3 getColor(){return color;}
	void print();
};
