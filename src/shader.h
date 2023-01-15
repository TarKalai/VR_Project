#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>

#include "directionalLight.h"
#include "pointLight.h"

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

    void CreateFromString(const char* vertexCode, const char* fragmentCode); //Create the shader from a string we pass into it without reading a file. 
    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation); 
    void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);


    std::string ReadFile(const char* fileLocation); 

    GLuint GetProjectionLocation(); 
    GLuint GetModelLocation(); 
    GLuint GetViewLocation();
    // GLuint GetFogColorLocation();  
    GLuint GetAmbientIntensityLocation(); 
    GLuint GetAmbientColorLocation(); 

    GLuint GetDiffuseIntensityLocation(); 
    GLuint GetDirectionLocation(); 
    GLuint GetSpecularIntensityLocation(); 
    GLuint GetShininessLocation(); 
    GLuint GetEyePositionLocation();

    GLuint GetUniformSkyboxDay(); 
    GLuint GetUniformSkyboxNight(); 
    GLuint GetUniformBlendFactor(); 
    GLuint GetUniformSinTime(); 
    GLuint GetUniformCosTime(); 

    GLuint GetReflectivityLocation(); 
    GLuint GetRefractivityLocation(); 
    GLuint GetCoefRefractionLocation(); 


    GLuint GetOmniLightPosLocation(); 
    GLuint GetFarPlaneLocation(); 

    void SetDirectionalLight(DirectionalLight * dLight);
    void SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset); 
    void SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);

    void SetAreaLights(AreaLight *  aLights, int lightCount);
    void SetLTC1(GLuint textureUnit);
    void SetLTC2(GLuint textureUnit);

    void SetLightMatrices(std::vector<glm::mat4> lightMatrices); 

    
    void SetTexture(GLuint textureUnit); 
    void SetNormalMap(GLuint textureUnit); 
    void SetDepthMap(GLuint textureUnit); 
    void SetDirectionalShadowMap(GLuint textureUnit);
    void SetDirectionalLightTransform(glm::mat4* ltransform);  

    void ConnectSkyboxes(); 
    void SetBlendFactor(); 
    void SetTime();

    void addObject(Object *obj);
    void addObjects(std::vector<Object*> objects);


    void RenderPass(Camera camera, glm::mat4 projection, glm::mat4 view, 
                         DirectionalLight *mainLight, 
                         PointLight * pointLights, 
                         int pointLightCount, 
                         SpotLight * spotLights, 
                         int spotLightCount, 
                         AreaLight * areaLights, 
                         int areaLightCount); // RenderPass

    void RenderBump(Camera camera, glm::mat4 projection, glm::mat4 view, 
                    DirectionalLight* mainLight,
                    PointLight* pointLights, 
                    int pointLightCount, 
                    SpotLight* spotLights, 
                    int spotLightCount, 
                    AreaLight* areaLights, 
                    int areaLightCount);      

    void RenderParalax(Camera camera, glm::mat4 projection, glm::mat4 view, 
                         DirectionalLight* mainLight,
                         PointLight* pointLights, 
                         int pointLightCount, 
                         SpotLight* spotLights, 
                         int spotLightCount, 
                         AreaLight* areaLights, 
                         int areaLightCount);

    void DirectionalShadowMapPass(DirectionalLight* light); // DirectionalShadowMapPass
    void DrawLightObjects(glm::mat4 projection, glm::mat4 view); // DrawLightObjects
    void RenderScene(); // RenderScene

    void Validate(); 

    void CompileProgram(); 

    void UseShader(); 
    void remove(int objID);
    void ClearShader();


    void SetSkyColor(float r, float g, float b); 
 


    ~Shader(); 
private: 

    int pointLightCount;
    int spotLightCount; 
    int areaLightCount;

    GLuint uniformProjection, uniformModel, uniformView, uniformEyePosition, 
    uniformSpecularIntensity, uniformShininess,
    uniformTexture, uniformColor,
    uniformDirectionalLightTransform, uniformDirectionalShadowMap, 
    uniformNormalMap, uniformSkyColor, uniformDepthMap,
    uniformSinTime, uniformCosTime,
    uniformSkyboxDay, uniformSkyboxNight, uniformBlendFactor, 
    uniformReflectivity, uniformRefractivity, uniformCoefRefraction,
    
    uniformOmniLightPos, uniformFarPlane, uniformAlbedoRoughness;  

    GLuint uniformLightMatrices[6]; 


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

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode); 

    void AddShader(GLuint program, const char* shader_code, GLenum shader_type); 

};