#include "shader.h"
#include "glm/gtx/string_cast.hpp" // (print matrix) debug purpose
#include <vector>
#include <string>

Shader::Shader(const char* vertexPath, const char* fragmentPath, bool texture, bool normal)
	{   
        // texturePath = imagePath;
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

void Shader::setVec3(const GLchar* name, const glm::vec3 &value){ 
    glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]); 
}
    
void Shader::setMatrix4(const GLchar* name, const glm::mat4& matrix) 
    {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
void Shader::addObject(Object *obj) {
    obj->MakeObject(ID, shaderTexture, shaderNormal);
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

    if(lightCount > values::MAX_POINT_LIGHTS) lightCount = values::MAX_POINT_LIGHTS;

    uniformPointLightCount = glGetUniformLocation(ID, "pointLightCount");

    glUniform1i(uniformPointLightCount, lightCount); // make sure it is an int ! to go through the loop

    for (int i = 0; i < values::MAX_POINT_LIGHTS; i ++){

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

    if(lightCount > values::MAX_SPOT_LIGHTS) lightCount = values::MAX_SPOT_LIGHTS; 

    uniformSpotLightCount = glGetUniformLocation(ID, "spotLightCount"); 

    glUniform1i(uniformSpotLightCount, lightCount); // make sure it is an int ! to go through the loop


    for (int i = 0; i < values::MAX_SPOT_LIGHTS; i ++){

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

glm::mat4 Shader::getModelRotPos(glm::vec3 obj_pos, glm::vec3 obj_rot, glm::vec3 scale){
    glm::vec3 position = obj_pos;
    glm::vec3 rotation = obj_rot;
    float x1 = (cos(rotation.y)*cos(rotation.z)) * scale.x; 
    float x2 = (sin(rotation.x)*sin(rotation.y)*cos(rotation.z) - cos(rotation.x)*sin(rotation.z)) * scale.y;
    float x3 = (cos(rotation.x)*sin(rotation.y)*cos(rotation.z) + sin(rotation.x)*sin(rotation.z)) * scale.z;
    float y1 = (cos(rotation.y)*sin(rotation.z)) * scale.x;
    float y2 = (sin(rotation.x)*sin(rotation.y)*sin(rotation.z) + cos(rotation.x)*cos(rotation.z)) * scale.y;
    float y3 = (cos(rotation.x)*sin(rotation.y)*sin(rotation.z) - sin(rotation.x)*cos(rotation.z)) * scale.z;
    float z1 = (-sin(rotation.y)) * scale.x;
    float z2 = (sin(rotation.x)*cos(rotation.y)) * scale.y;
    float z3 = (cos(rotation.x)*cos(rotation.y)) * scale.z;
    glm::mat4 model = glm::mat4(x1,y1,z1,0,x2,y2,z2,0,x3,y3,z3,0,position.x,position.y,position.z,1);
    return model;
}


void Shader::SetAreaLights(AreaLight *  aLights, unsigned int lightCount){
    if(lightCount > values::MAX_AREA_LIGHTS) lightCount = values::MAX_AREA_LIGHTS; 
    
    setInteger("areaLightCount", lightCount);
    setInteger("LTC1", 0);
    setInteger("LTC2", 1);
	setInteger("material.diffuse", 0);

    for (int i = 0; i < lightCount; i++){

        // char locBuff[100] = {'\0'}; //setting all values to \0 which is EOS (End Of String)
        std::vector<glm::vec3> vecpos = aLights[i].getVertexPosition();
        // printf("here is the values of the vec pos 0 %f %f %f\n", vecpos[0].x, vecpos[0].y, vecpos[0].z);
        // glm::mat4 model(1.0f);
        // model = glm::translate(model, aLights[i].getPosition());
		// model =  glm::rotate(model, glm::radians(-90.0) , glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 model = getModelRotPos(aLights[i].getPosition(), aLights[i].getRotation(), glm::vec3(1.0));
        glm::vec3 p0 = glm::vec3(model * glm::vec4(vecpos[3], 1.0f));
		glm::vec3 p1 = glm::vec3(model * glm::vec4(vecpos[1], 1.0f));
		glm::vec3 p2 = glm::vec3(model * glm::vec4(vecpos[0], 1.0f));
		glm::vec3 p3 = glm::vec3(model * glm::vec4(vecpos[2], 1.0f));
        std::string str_points = "areaLights[" + std::to_string(i) + "].points";
		std::string str_color = "areaLights[" + std::to_string(i) + "].base.color";
		std::string str_intensity = "areaLights[" + std::to_string(i) + "].base.diffuseIntensity";
		std::string str_twoSided = "areaLights[" + std::to_string(i) + "].twoSided";

        setVec3((str_points + "[0]").c_str(), p0);
		setVec3((str_points + "[1]").c_str(), p1);
		setVec3((str_points + "[2]").c_str(), p2);
		setVec3((str_points + "[3]").c_str(), p3);
		setVec3(str_color.c_str(), aLights[i].getColor());
		setFloat(str_intensity.c_str(), 2.0f); // value for domino : 2.f && value for ground : 10.f
		setInteger(str_twoSided.c_str(), 1);

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

void Shader::DrawObjects(glm::mat4 view, 
                         glm::mat4 projection, 
                         glm::vec3 position_cam, glm::vec3 front_cam, 
                         DirectionalLight* mainLight, 
                         PointLight * pLights, 
                         unsigned int pLightCount, 
                         SpotLight * sLights, 
                         unsigned int sLightCount, 
                         AreaLight * aLights, 
                         unsigned int aLightCount,
                         Material material) {
    use();
    setMatrix4("view", view); //V
    setMatrix4("projection", projection); //P
    setVector3f("eyePosition", position_cam);
    SetDirectionalLight(mainLight);
    SetPointLights(pLights, pLightCount);
    SetSpotLights(sLights, sLightCount); 
    SetAreaLights(aLights, aLightCount);

    glm::vec3 lowerLight = position_cam; 
    lowerLight.y -= 0.3f;
	sLights[0].SetFlash(lowerLight, front_cam);

    for(Object* object : objectList) {
        if (object->visible) {
            setMatrix4("model", object->model);
            material.UseMaterial(glGetUniformLocation(ID, "material.specularIntensity"), glGetUniformLocation(ID, "material.shininess")); 
            // setMatrix4("itM", glm::inverseTranspose(object->model));
            object->draw();
        }
    }
}

void Shader::DrawLightObjects(glm::mat4 view, 
                              glm::mat4 projection, 
                              AreaLight * aLights, 
                              unsigned int aLightCount){
    use();
    setMatrix4("view", view); //V
    setMatrix4("projection", projection); //P
    int i = 0;
    for(Object* object : objectList) {
        setMatrix4("model", object->model);
        setVector3f("lightColor", aLights[i].getColor());
		// setMatrix4("itM", glm::inverseTranspose(object->model));
        object->draw();
        i += 1;
    }
}
