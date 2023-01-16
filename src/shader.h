#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>

#include "directionalLight.h"
#include "pointLight.h"
#include "light_constructor.h"

#include "constant.h"
#include "spotLight.h"
#include "areaLight.h"
#include "object.h"
#include "camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <vector>


class Shader
{
public:
    std::vector<Object*> objectList;
    GLuint shaderID;

    Shader(); 

    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation); 
    void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

    void RenderPass(Camera camera, glm::mat4 projection, glm::mat4 view, LightConstructor* lightConstructor); // RenderPass
    void RenderBump(Camera camera, glm::mat4 projection, glm::mat4 view, LightConstructor* lightConstructor);      
    void RenderParalax(Camera camera, glm::mat4 projection, glm::mat4 view, LightConstructor* lightConstructor);
    void DirectionalShadowMapPass(LightConstructor* lightConstructor); // DirectionalShadowMapPass
    void OmniShadowMapPass(LightConstructor* lightConstructor);
    void DrawLightObjects(glm::mat4 projection, glm::mat4 view); // DrawLightObjects

    void addObject(Object *obj);
    void addObjects(std::vector<Object*> objects);
    void remove(int objID);
    void ClearShader();
    void Validate(); 

    // SETTERS

    void SetDirectionalLight(DirectionalLight * dLight);
    void SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset); 
    void SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
    void SetAreaLights(AreaLight *  aLights, int lightCount);
    void SetLightMatrices(std::vector<glm::mat4> lightMatrices); 
    void SetTime();
    void SetSunLightColor(); 
    void SetBlendFactor(); 
    void ConnectSkyboxes(); 

    void SetLTC1(GLuint textureUnit){glUniform1i(uniformLTC1, textureUnit);}
    void SetLTC2(GLuint textureUnit){glUniform1i(uniformLTC2, textureUnit);}
    void SetSkyColor(float r, float g, float b){glUniform3f(uniformSkyColor, r, g, b); }
    void SetTexture(GLuint textureUnit){glUniform1i(uniformTexture, textureUnit);}
    void SetNormalMap(GLuint textureUnit){glUniform1i(uniformNormalMap, textureUnit);}
    void SetDepthMap(GLuint textureUnit){glUniform1i(uniformDepthMap, textureUnit);}
    void SetDirectionalShadowMap(GLuint textureUnit){glUniform1i(uniformDirectionalShadowMap, textureUnit);}
    void SetDirectionalLightTransform(glm::mat4* lTransform){glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));}
 
    // GETTERS

    // General 
    GLuint GetProjectionLocation(){return uniformProjection;}
    GLuint GetModelLocation(){return uniformModel;}
    GLuint GetViewLocation(){return uniformView;}
    GLuint GetEyePositionLocation(){return uniformEyePosition;}
    // Skybox & Reflection
    GLuint GetUniformSkyboxDay(){return uniformSkyboxDay;}
    GLuint GetUniformSkyboxNight(){return uniformSkyboxNight;}
    GLuint GetUniformBlendFactor(){return uniformBlendFactor;}
    GLuint GetUniformSunLightColor(){return uniformSunLightColor;}
    GLuint GetUniformSinTime(){return uniformSinTime;}
    GLuint GetUniformCosTime(){return uniformCosTime;}
    GLuint GetReflectivityLocation(){return uniformReflectivity;} 
    GLuint GetRefractivityLocation(){return uniformRefractivity;} 
    GLuint GetCoefRefractionLocation(){return uniformCoefRefraction;}
    // Light & Material
    GLuint GetAmbientColorLocation(){return uniformDirectionalLight.uniformColor;}
    GLuint GetAmbientIntensityLocation(){return uniformDirectionalLight.uniformAmbientIntensity;}
    GLuint GetDiffuseIntensityLocation(){return uniformDirectionalLight.uniformDiffuseIntensity;}
    GLuint GetDirectionLocation(){return uniformDirectionalLight.uniformDirection;}
    GLuint GetSpecularIntensityLocation(){return uniformSpecularIntensity;}
    GLuint GetShininessLocation(){return uniformShininess;}
    GLuint GetOmniLightPosLocation(){return uniformOmniLightPos;}
    GLuint GetFarPlaneLocation(){return uniformFarPlane;}

    ~Shader(); 
private: 
    GLuint uniformProjection, uniformModel, uniformView, uniformEyePosition, 
    uniformSpecularIntensity, uniformShininess,
    uniformTexture, uniformColor,
    uniformDirectionalLightTransform, uniformDirectionalShadowMap, 
    uniformNormalMap, uniformSkyColor, uniformDepthMap,
    uniformSinTime, uniformCosTime, uniformSunLightColor,
    uniformSkyboxDay, uniformSkyboxNight, uniformBlendFactor, 
    uniformReflectivity, uniformRefractivity, uniformCoefRefraction,
    uniformOmniLightPos, uniformFarPlane, uniformAlbedoRoughness;  

    GLuint uniformLightMatrices[6]; 

    void RenderScene();
    void UseShader();
    void UseShaderAndLink(Camera camera, glm::mat4 projection, glm::mat4 view, LightConstructor* light); 
    void CompileProgram(); 
    std::string ReadFile(const char* fileLocation); 
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode); 

    void AddShader(GLuint program, const char* shader_code, GLenum shader_type); 
    void PointShadowMapPass(PointLight* light);


    struct {
        GLuint uniformColor; 
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 
        GLuint uniformDirection; 
    } uniformDirectionalLight; // struct calls uniformDirectionalLight, it is an instance of the struct (which does not have a name)

    GLuint uniformPointLightCount, uniformMaxPointLight; 

    

    struct {
        GLuint uniformColor; 
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformPosition;
        GLuint uniformConstant; 
        GLuint uniformLinear; 
        GLuint uniformExponent; 
    } uniformPointLight[values::MAX_POINT_LIGHTS]; // We are going to have mulitple lights, so we use an array

    GLuint uniformSpotLightCount, uniformMaxSpotLight; 

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

    GLuint uniformLTC1;
    GLuint uniformLTC2;
    GLuint uniformAreaLightCount, uniformMaxAreaLight ;
    struct {
        GLuint uniformColor; 
        GLuint uniformIntensity; 
        GLuint uniformTwoSided;

        GLuint uniformPoint0;
        GLuint uniformPoint1;
        GLuint uniformPoint2;
        GLuint uniformPoint3;
    } uniformAreaLight[values::MAX_AREA_LIGHTS];


    struct { // struct that represents the struct in the shader.frag
        GLuint shadowMap; 
        GLuint farPlane; 
    } uniformOmniShadowMap[values::MAX_POINT_LIGHTS + values::MAX_SPOT_LIGHTS];
};