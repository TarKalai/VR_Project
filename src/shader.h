#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include<glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath); 

	void use(); 

    void setInteger(const GLchar *name, GLint value); 

    void setFloat(const GLchar* name, GLfloat value);

    void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);

    void setVector3f(const GLchar* name, const glm::vec3& value);

    void setMatrix4(const GLchar* name, const glm::mat4& matrix); 


private:
    GLuint compileShader(std::string shaderCode, GLenum shaderType);

    GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader); 

};
#endif