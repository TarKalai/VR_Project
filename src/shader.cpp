#include "shader.h"

#include "glm/gtx/string_cast.hpp" // (print matrix) debug purpose

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


void Shader::SetDirectionalLight(DirectionalLight * dLight){

    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(ID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformColor =   glGetUniformLocation(ID, "directionalLight.base.color"); 
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(ID, "directionalLight.base.diffuseIntensity"); 
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(ID, "directionalLight.direction");

    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity,uniformDirectionalLight.uniformColor,
    uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount){

    if(lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    uniformPointLightCount = glGetUniformLocation(ID, "pointLightCount");

    glUniform1i(uniformPointLightCount, lightCount); // make sure it is an int ! to go through the loop

    for (int i = 0; i < MAX_POINT_LIGHTS; i ++){

        char locBuff[100] = {'\0'}; //setting all values to \0 which is EOS (End Of String)

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i); // we are going to print to a buffer
        uniformPointLight[i].uniformColor = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i); // we are going to print to a buffer
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i); // we are going to print to a buffer
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i); // we are going to print to a buffer
        uniformPointLight[i].uniformPosition = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i); // we are going to print to a buffer
        uniformPointLight[i].uniformConstant = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i); // we are going to print to a buffer
        uniformPointLight[i].uniformLinear = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i); // we are going to print to a buffer
        uniformPointLight[i].uniformExponent = glGetUniformLocation(ID, locBuff); 
    }


    for(int i=0; i < lightCount; i++){
        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, 
                        uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
                        uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent); 
    }

}

void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount){

    if(lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS; 

    uniformSpotLightCount = glGetUniformLocation(ID, "spotLightCount"); 

    glUniform1i(uniformSpotLightCount, lightCount); // make sure it is an int ! to go through the loop


    for (int i = 0; i < MAX_SPOT_LIGHTS; i ++){

        char locBuff[100] = {'\0'}; //setting all values to \0 which is EOS (End Of String)

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformColor = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(ID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(ID, locBuff); 
    }

    for(int i=0; i < lightCount; i++){

        sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, 
                        uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition,uniformSpotLight[i].uniformDirection, 
                        uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
                        uniformSpotLight[i].uniformEdge); 
    }
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


void Shader::SetTexture(GLuint textureUnit){
    uniformTexture = glGetUniformLocation(ID, "theTexture");
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit){
    uniformDirectionalShadowMap = glGetUniformLocation(ID, "directionalShadowMap"); 
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform){
    uniformDirectionalLightTransform = glGetUniformLocation(ID, "directionalLightTransform");
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform)); 
}


void Shader::DirectionalShadowMapPass(DirectionalLight* light){ // we have a pointer to the directional light we are having shadows from
// handles the shadowMap pass 

    use(); // doesn't have colors, it is the depth map. 

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); // we need to make sure the buffer we are drawing to is the same size as the viewport

    light->GetShadowMap()->Write();

    glClear(GL_DEPTH_BUFFER_BIT); // if there iis already some depth infor. in the buffer we clear it

    uniformModel  =  glGetUniformLocation(ID, "model");

    glm::mat4 resLight = light->CalculateLightTransform(); 
    SetDirectionalLightTransform(&resLight);

    // RenderScene(); 

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // attach the default buffer

}

void Shader::DrawObjects(glm::mat4 view, 
                         glm::mat4 projection, 
                         glm::vec3 position_cam, glm::vec3 front_cam, 
                         DirectionalLight* mainLight, 
                         GLuint uniformSpecularIntensity, 
                         GLuint uniformShininess, 
                         PointLight * pLights, 
                         unsigned int pLightCount, 
                         SpotLight * sLights, 
                         unsigned int sLightCount){
    use();


    glViewport(0, 0, 1366, 768); 

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // A pixel does not only have color as data, it also has depth and other things. We are specifying here that we want to clear the color. 
    //glClear is also clearing the depth buffer bit.

    setMatrix4("projection", projection); //P
    setMatrix4("view", view); //V
    setFloat("material.specularIntensity", uniformSpecularIntensity); 
    setFloat("material.shininess",uniformShininess); 
    setVector3f("eyePosition", position_cam);
    SetDirectionalLight(mainLight); // chenged to &mainLight
    SetPointLights(pLights, pLightCount);
    SetSpotLights(sLights, sLightCount); 

    // shadow 
    glm::mat4 resmainLight = mainLight->CalculateLightTransform();
    SetDirectionalLightTransform(&resmainLight); 

    mainLight->GetShadowMap()->Read(GL_TEXTURE1);

    SetTexture(0); // bound to texture unit 0 
    SetDirectionalShadowMap(1); // bound to GL_TEXTURE1

    glm::vec3 lowerLight = position_cam; 
    lowerLight.y -= 0.3f;
	sLights[0].SetFlash(lowerLight, front_cam);

    for(Object* object : objectList) {
        if (object->visible) {
            setMatrix4("model", object->model);
            object->draw();
        }
    }
}