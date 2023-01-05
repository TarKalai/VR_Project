#include "shader2D.h"

Shader2D::Shader2D(bool test) {
    const std::string sourceV = "#version 330 core\n"
		"in vec3 position; \n"
		"in vec3 color; \n"
		"out vec4 v_color; \n"
		"uniform mat4 M; \n"
        
		" void main(){ \n"
		"gl_Position = M*vec4(position, 2);\n"
		"v_color = vec4(color, 1.0);\n"

		"}\n"; 
	const std::string sourceF = "#version 330 core\n"
		"out vec4 FragColor; \n"
		"precision mediump float; \n"
		"in vec4 v_color;\n"
		"void main() { \n"
		"FragColor = v_color; \n"
		"} \n";

	GLuint shaderV = compileShader(sourceV, GL_VERTEX_SHADER);
	GLuint shaderF = compileShader(sourceF, GL_FRAGMENT_SHADER);
	program = compileProgram(shaderV, shaderF);
    addCross();
}

void Shader2D::addCross() {
	// First object!
	const float positionsData[18] = {
		// vertices			// colors
		-.10, -.10, 0.0, 0.0, 0.0, 0.0,
		.10, -.10, 0.0, 0.0, 0.0, 0.0,
		0.0, .10, 0.0, 0.0, 0.0, 0.0,
	};

	//generate the buffer and the vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//define VBO and VAO as active buffer and active vertex array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionsData), positionsData, GL_STATIC_DRAW);

	auto attribute = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);

	auto att_color = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(att_color);
	glVertexAttribPointer(att_color, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	//desactive the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//1. Matrix for the model, view and projection
	model = glm::mat4(1.0);
	model = glm::translate(model,glm::vec3(0,0,0));
}

void Shader2D::drawObject(){
    glBindVertexArray(VAO);
		
    glUseProgram(program);

    auto u_model = glGetUniformLocation(program, "M");
    //4. Use the model, view and projection matrices in the vertex shader
    glUniformMatrix4fv(u_model,1,GL_FALSE,glm::value_ptr(model));
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


GLuint Shader2D::compileShader(std::string shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	const char* code = shaderCode.c_str();
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

	GLchar infoLog[1024];
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::string t = "undetermined";
		if (shaderType == GL_VERTEX_SHADER) {
			t = "vertex shader";
		}
		else if (shaderType == GL_FRAGMENT_SHADER) {
			t = "fragment shader";
		}
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of the " << t << ": " << shaderType << infoLog << std::endl;
	}
	return shader;
}

GLuint Shader2D::compileProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programID = glCreateProgram();

	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);


	GLchar infoLog[1024];
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR:  " << infoLog << std::endl;
	}
	return programID;
}