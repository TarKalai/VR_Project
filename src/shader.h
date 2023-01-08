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

#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "areaLight.h"
#include "constant.h"
#include "material.h"


class Shader
{
public:
	GLuint ID;

    std::vector<Object*> objectList;
    
    bool shaderTexture;
    bool shaderNormal;
    // char * texturePath;

    Shader( const char* vertexPath, const char* fragmentPath, bool texture, bool normal); //char* imagePath,

	void use(); 

    void setInteger(const GLchar *name, GLint value); 

    void setFloat(const GLchar* name, GLfloat value);

    void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);

    void setVector3f(const GLchar* name, const glm::vec3& value);

    void setVec3(const GLchar* name, const glm::vec3 &value);

    void setMatrix4(const GLchar* name, const glm::mat4& matrix); 

    void SetDirectionalLight(DirectionalLight * dLight); 

    void SetPointLights(PointLight * pLight, unsigned int lightCount);

    void SetSpotLights(SpotLight * sLight, unsigned int lightCount); 

    void SetAreaLights(AreaLight *  aLight, unsigned int lightCount);

    glm::mat4 getModelRotPos(glm::vec3 obj_pos, glm::vec3 obj_rot, glm::vec3 scale);

    // void setColor(const GLchar* name, glm::vec3& color);

    void addObject(Object* obj);

    void DrawObjects(glm::mat4 view, 
                     glm::mat4 projection, 
                     glm::vec3 position_cam, glm::vec3 front_cam,
                     DirectionalLight* mainLight, 
                     PointLight * pLights, 
                     unsigned int pLightCount, 
                     SpotLight * sLights, 
                     unsigned int sLightCount,
                     AreaLight * aLights, 
                     unsigned int aLightCount,
                     Material material); //, glm::vec3 light_pos);
    
    void DrawLightObjects(glm::mat4 view, 
                          glm::mat4 projection,
                          AreaLight * aLights, 
                          unsigned int aLightCount);

    GLuint uniformPointLightCount;
    struct {
        GLuint uniformColor; 
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformPosition;
        GLuint uniformConstant; 
        GLuint uniformLinear; 
        GLuint uniformExponent; 
    } uniformPointLight[values::MAX_POINT_LIGHTS];

    struct {
        GLuint uniformColor; 
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 
        GLuint uniformDirection; 
    } uniformDirectionalLight;

    GLuint uniformSpotLightCount;

    struct {
        GLuint uniformColor; 
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformPosition;
        GLuint uniformConstant; 
        GLuint uniformLinear; 
        GLuint uniformExponent;
        
        GLuint uniformDirection; 
        GLuint uniformEdge; 
    } uniformSpotLight[values::MAX_SPOT_LIGHTS];

    GLuint uniformAreaLightCount;
    struct {
        GLuint uniformColor; 
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformPosition;
        GLuint uniformConstant; 
        GLuint uniformLinear; 
        GLuint uniformExponent; 
    } uniformAreaLight[values::MAX_AREA_LIGHTS];



private:
    GLuint compileShader(std::string shaderCode, GLenum shaderType);

    GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader); 

};
#endif