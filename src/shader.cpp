#include "shader.h"

Shader::Shader(char *imagePath, const char* vertexPath, const char* fragmentPath, bool texture, bool normal)
	{   
        texturePath = imagePath;
        shaderTexture = texture;
        shaderNormal = normal;
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        GLuint vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
        GLuint fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
        ID = compileProgram(vertex, fragment);
	}

void Shader::use() 
    {
        glUseProgram(ID);
    }

void Shader::setInteger(const GLchar *name, GLint value) 
    {
    glUniform1i(glGetUniformLocation(ID, name), value);
    }

void Shader::setFloat(const GLchar* name, GLfloat value)
    {
    glUniform1f(glGetUniformLocation(ID, name), value);
    }

void Shader::setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) 
    {
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
    }

void Shader::setVector3f(const GLchar* name, const glm::vec3& value) 
    {
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
    }
    
void Shader::setMatrix4(const GLchar* name, const glm::mat4& matrix) 
    {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

void Shader::addObject(Object *obj) {
    obj->MakeObject(ID, shaderTexture, shaderNormal, texturePath);
    objectList.push_back(obj);
}


GLuint Shader::compileShader(std::string shaderCode, GLenum shaderType)
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

GLuint Shader::compileProgram(GLuint vertexShader, GLuint fragmentShader)
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

void Shader::DrawObjects(glm::mat4 view, glm::mat4 perspective, glm::vec3 light_pos, glm::vec3 camera_position) {
    use();
    double now = glfwGetTime();
    setMatrix4("V", view);
    setMatrix4("P", perspective);
    setVector3f("u_view_pos", camera_position);
    auto delta = light_pos + glm::vec3(0.0,0.0,2 * std::sin(now));
		std::cout << delta.z <<std::endl;
	setVector3f("light.light_pos", delta);
    int i = 0;
    for(Object* object : objectList) {
        i += 1;
        setMatrix4("M", object->model);
		setMatrix4("itM", glm::inverseTranspose(object->model));
        object->draw();
    }
}