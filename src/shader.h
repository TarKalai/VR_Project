#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector> 

#include "object.h"

class Shader
{
public:
	GLuint ID;

    std::vector<Object*> objectList;
    
    bool shaderTexture;
    bool shaderNormal;
    char * texturePath;

    Shader(char* imagePath, const char* vertexPath, const char* fragmentPath, bool texture, bool normal); 

	void use(); 

    void setInteger(const GLchar *name, GLint value); 

    void setFloat(const GLchar* name, GLfloat value);

    void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);

    void setVector3f(const GLchar* name, const glm::vec3& value);

    void setMatrix4(const GLchar* name, const glm::mat4& matrix); 

    void addObject(Object* obj);

    void DrawObjects(glm::mat4 view, glm::mat4 perspective, glm::vec3 light_pos);




private:
    GLuint compileShader(std::string shaderCode, GLenum shaderType);

    GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader); 

};
#endif