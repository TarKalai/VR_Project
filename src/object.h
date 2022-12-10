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

#include "shader.h"

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

	std::vector<glm::vec3> vt_positions;
	std::vector<glm::vec2> vt_textures;
	std::vector<glm::vec3> vt_normals;
	std::vector<Vertex> vertices;

	int numVertices;

	GLuint VBO, VAO;

	glm::mat4 model = glm::mat4(1.0);


	Object(const char* path, glm::vec3 p, float s) {

		position = p;
		scale = s;

		// Read the file defined by the path argument 
		// open the .obj file into a text editor and see how the data are organized
		// you will see line starting by v, vt, vn and f --> what are these ?
		// Then parse it to extract the data you need
		// keep track of the number of vertices you need

		std::ifstream infile(path);
		//TODO Error management
		std::string line;
		while (std::getline(infile, line))
		{
			std::istringstream iss(line); // Like a "split"
			std::string indice;
			iss >> indice; // Go the next elem of the split (reach elem 1 by 1 separated by " ")
			//std::cout << "indice : " << indice << std::endl;
			if (indice == "v") {
				float x, y, z;
				iss >> x >> y >> z;
				vt_positions.push_back(glm::vec3(x, y, z));

			}
			else if (indice == "vn") {
				float x, y, z;
				iss >> x >> y >> z;
				vt_normals.push_back(glm::vec3(x, y, z));
			}
			else if (indice == "vt") {
				float u, v;
				iss >> u >> v;
				vt_textures.push_back(glm::vec2(u, v));
			}
			else if (indice == "f") {
				std::string f1, f2, f3;
				iss >> f1 >> f2 >> f3;

				std::string p, t, n;

				//for face 1
				Vertex v1;

				p = f1.substr(0, f1.find("/"));
				f1.erase(0, f1.find("/") + 1);

				t = f1.substr(0, f1.find("/"));
				f1.erase(0, f1.find("/") + 1);

				n = f1.substr(0, f1.find("/"));


				v1.Position = vt_positions.at(std::stof(p) - 1);
				v1.Normal = vt_normals.at(std::stof(n) - 1);
				v1.Texture = vt_textures.at(std::stof(t) - 1);
				vertices.push_back(v1);

				//for face 2
				Vertex v2;

				p = f2.substr(0, f2.find("/"));
				f2.erase(0, f2.find("/") + 1);

				t = f2.substr(0, f2.find("/"));
				f2.erase(0, f2.find("/") + 1);

				n = f2.substr(0, f2.find("/"));


				v2.Position = vt_positions.at(std::stof(p) - 1);
				v2.Normal = vt_normals.at(std::stof(n) - 1);
				v2.Texture = vt_textures.at(std::stof(t) - 1);
				vertices.push_back(v2);

				//for face 3
				Vertex v3;

				p = f3.substr(0, f3.find("/"));
				f3.erase(0, f3.find("/") + 1);

				t = f3.substr(0, f3.find("/"));
				f3.erase(0, f3.find("/") + 1);

				n = f3.substr(0, f3.find("/"));


				v3.Position = vt_positions.at(std::stof(p) - 1);
				v3.Normal = vt_normals.at(std::stof(n) - 1);
				v3.Texture = vt_textures.at(std::stof(t) - 1);
				vertices.push_back(v3);
			}
		}
		//std::cout << positions.size() << std::endl;
		//std::cout << vt_normals.size() << std::endl;
		//std::cout << vt_textures.size() << std::endl;
		std::cout << "Load model with " << vertices.size() << " vertices" << std::endl;

		infile.close();

		numVertices = vertices.size();
	}



	void makeObject(Shader shader, bool position = true, bool normal = true, bool texture = true) {


		//Create the VAO and VBO
		//Put your data into your VBO
		//Define VBO and VAO as active buffer and active vertex array
		//Use the VAO to specify how your data should be read by your shader (glVertexAttribPointer and co)
		//Sometimes your shader will not use texture or normal attribute
		//you can use the boolean defined above to not specify these attribute 
		//desactive the buffer and delete the datas when your done

		float* data = new float[8 * numVertices];
		for (int i = 0; i < numVertices; i++) {
			Vertex v = vertices.at(i);
			data[i * 8] = v.Position.x;
			data[i * 8 + 1] = v.Position.y;
			data[i * 8 + 2] = v.Position.z;

			data[i * 8 + 3] = v.Texture.x;
			data[i * 8 + 4] = v.Texture.y;

			data[i * 8 + 5] = v.Normal.x;
			data[i * 8 + 6] = v.Normal.y;
			data[i * 8 + 7] = v.Normal.z;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		//define VBO and VAO as active buffer and active vertex array
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, data, GL_STATIC_DRAW);

		if (position) {
			auto att_pos = glGetAttribLocation(shader.ID, "position");
			glEnableVertexAttribArray(att_pos);
			glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
		}
		
		if (texture) {
			auto att_tex = glGetAttribLocation(shader.ID, "tex_coord");
			glEnableVertexAttribArray(att_tex);
			glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			
		}
		
		if (normal) {
			auto att_col = glGetAttribLocation(shader.ID, "normal");
			glEnableVertexAttribArray(att_col);
			glVertexAttribPointer(att_col, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		}
		//desactive the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		delete[] data;

	}

	void draw() {

		//bind your vertex arrays and call glDrawArrays
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

	}
};
#endif