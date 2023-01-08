#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector> 

#include "object.h"

#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "commonValues.h"
#include "display.h"


class Shader
{
public:
	GLuint ID;

    Display display; 

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

    void SetDirectionalLight(DirectionalLight * dLight); 

    void SetPointLights(PointLight * pLight, unsigned int lightCount);

    void SetSpotLights(SpotLight * sLight, unsigned int lightCount); 

    void addObject(Object* obj, bool shadow=false);

    void AddShader(GLuint program, const char* shader_code, GLenum shader_type); 

    void SetTexture(GLuint textureUnit); 
    void SetDirectionalShadowMap(GLuint textureUnit);
    void SetDirectionalLightTransform(glm::mat4* ltransform);  
    void DirectionalShadowMapPass(DirectionalLight* light); 

    void DrawObjects(glm::mat4 view, 
                     glm::mat4 projection, 
                     glm::vec3 position_cam, glm::vec3 front_cam,
                     DirectionalLight mainLight, 
                     GLuint uniformSpecularIntensity, 
                     GLuint uniformShininess, 
                     PointLight * pLights, 
                     unsigned int pLightCount, 
                     SpotLight * sLights, 
                     unsigned int sLightCount); //, glm::vec3 light_pos);

    GLuint uniformPointLightCount;
    struct {
        GLuint uniformColor; 
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformPosition;
        GLuint uniformConstant; 
        GLuint uniformLinear; 
        GLuint uniformExponent; 
    } uniformPointLight[MAX_POINT_LIGHTS];

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
    } uniformSpotLight[MAX_SPOT_LIGHTS];
    
    void ClearShader(); 

    ~Shader(); 


private:

    GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition, 
    uniformSpecularIntensity, uniformShininess,
    uniformTexture, 
    uniformDirectionalLightTransform, uniformDirectionalShadowMap;
    GLuint compileShader(std::string shaderCode, GLenum shaderType);

    GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader); 

};
#endif