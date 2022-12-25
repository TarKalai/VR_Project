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

#include "stb_image.h"


struct Vertex {
	glm::vec3 Position;
	glm::vec2 Texture;
	glm::vec3 Normal;
};

class Object
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	// VAO object, VBO shader
	int id;
	GLuint VAO, VBO;

	bool has_texture;
	GLuint u_texture;
	GLuint texture;

	std::vector<glm::vec3> vt_positions;
	std::vector<glm::vec2> vt_textures;
	std::vector<glm::vec3> vt_normals;
	std::vector<Vertex> vertices;

	int width, height, nrChannels;


    unsigned char *data = stbi_load("../../image/iceland_heightmap.png", &width, &height, &nrChannels, 0); 
	//  = stbi_load("../../image/iceland_heightmap.png", &width, &height, &nrChannels, 0);
	// can be done in .cpp // create constructor for the floor ? 


	int numVertices;

    int rez = 1;

	const int numStrips = (height-1)/rez;
    const int numTrisPerStrip = (width/rez)*2-2;

	glm::mat4 model = glm::mat4(1.0);

	Object();
	
	Object(const char* path, glm::vec3 obj_pos, glm::vec3 obj_rot, glm::vec3 obj_scale, int identifier);
	
	void MakeObject(GLuint shaderID, bool shader_texture, bool shader_normal, char* texturePath);

	void setPosRot(glm::vec3 obj_pos, glm::vec3 obj_rot);

	void draw();

	void print();
};
#endif