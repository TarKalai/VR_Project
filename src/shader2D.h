#ifndef SHADER2D_H
#define SHADER2D_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>

#include <string>
#include <iostream>


class Shader2D
{
public:
    GLuint program;
    GLuint VBO, VAO;
    glm::mat4 model;

    Shader2D(bool test);

    void addCross(); 
    void drawObject();

    GLuint compileShader(std::string shaderCode, GLenum shaderType);
    GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif