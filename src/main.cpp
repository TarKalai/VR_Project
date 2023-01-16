#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //use type_ptr to pass the values to the shaders
#include "glm/gtx/string_cast.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "physics.h"
#include "shader.h"
#include "camera.h"
#include "light_constructor.h"
#include "object_constructor.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "areaLight.h"
#include "constant.h"
#include "object.h"
#include "process.h"
#include "gui.h"
#include "utils.h"
#include "shader2D.h"
#include "skybox.h"
#include "ltc_matrix.hpp"


Display mainWindow; 

PhysicalWorld physicalWorld;

Shader objectShader; 
Shader directionalShadowShader; 
Shader objectLightShader; 
Shader2D shader2D;
Shader bumpMapShader;
Shader paralaxMapShader;
Shader omniShadowShader; 

Camera camera; 

DirectionalLight* mainLight; 
PointLight *pointLights; 
SpotLight *spotLights; 
AreaLight *areaLights; 

std::vector<Object*> pointLightObjects;
std::vector<Object*> spotLightObjects;
std::vector<Object*> areaLightObjects;

int pointLightCount; 
int spotLightCount;
int areaLightCount;

GLuint uniformModel = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;  //TODO

void addToShaders(Object* obj) {
    if (obj->shaderType == ShaderType::POINTLIGHT || obj->shaderType == ShaderType::SPOTLIGHT || obj->shaderType == ShaderType::AREALIGHT) {
        objectLightShader.addObject(obj);
    }
    else {
        physicalWorld.addObject(obj);
        directionalShadowShader.addObject(obj); 
        omniShadowShader.addObject(obj);
        if (obj->shaderType == ShaderType::OBJECT)
            objectShader.addObject(obj);
        if (obj->shaderType == ShaderType::BUMPMAP)
            bumpMapShader.addObject(obj);
        else if (obj->shaderType == ShaderType::PARALLAX)
            paralaxMapShader.addObject(obj);
    }
}

void CreateObjects(){
    // GROUNDS
    Object* ground = new Object(geometry::plane3D, ShaderType::BUMPMAP, Textures::Brickwall(), Materials::Shiny(), PHYSIC::UNMOVABLE, glm::vec3(0., 0., 0.), glm::vec3(0.), glm::vec3(general::sceneSize.x/2., general::floorThickness, general::sceneSize.z/2), glm::vec3(1.));
    addToShaders(ground);

    Object* ground2 = new Object(geometry::plane3D, ShaderType::PARALLAX, Textures::Brick2(), Materials::Shiny(), PHYSIC::UNMOVABLE, glm::vec3(10., 5.0, 10.), glm::vec3(0.), glm::vec3(general::sceneSize.x/5., general::floorThickness, general::sceneSize.z/5), glm::vec3(1.));
    addToShaders(ground2);

    Object* box = new Object(geometry::cube, ShaderType::PARALLAX, Textures::Toy(), Materials::Shiny(), PHYSIC::MOVABLE, glm::vec3(10., 15.0, 5.), glm::vec3(0.), glm::vec3(8.0), glm::vec3(1.));
    addToShaders(box);
}

void CreateShaders()
{
    objectShader.CreateFromFiles(shaderfiles::mainVertex, shaderfiles::mainFrag); 
    directionalShadowShader.CreateFromFiles(shaderfiles::shadowMapVertex, shaderfiles::shadowMapFrag); 
    objectLightShader.CreateFromFiles(shaderfiles::lightObjectVertex, shaderfiles::lightObjectFrag); 
    shader2D = Shader2D(true);
    bumpMapShader.CreateFromFiles(shaderfiles::bumpMapVertex, shaderfiles::bumpMapFrag); 
    paralaxMapShader.CreateFromFiles(shaderfiles::paralaxMapVertex, shaderfiles::paralaxMapFrag);
    omniShadowShader.CreateFromFiles(shaderfiles::omniShadowVertex, shaderfiles::omniShadowGeom, shaderfiles::omniShadowFrag);
}

struct LTC_matrices {
	GLuint mat1;
	GLuint mat2;
};


GLuint loadTexture(const float *ltc)
{
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
	             0, GL_RGBA, GL_FLOAT, ltc);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}



void OmniShadowMapPass(PointLight* light)
{
    omniShadowShader.UseShader(); // doesn't have colors, it is the depth map. 

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); // we need to make sure the buffer we are drawing to is the same size as the viewport

    light->GetShadowMap()->Write();

    glClear(GL_DEPTH_BUFFER_BIT); // if there iis already some depth infor. in the buffer we clear it

    uniformModel  = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation(); 
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation(); 

    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z); // position of the light in the world
    glUniform1f(uniformFarPlane, light->GetFarPlane()); 

    // std::vector<glm::mat4> omiLightTransform = ; 
    omniShadowShader.SetLightMatrices(light->CalculateLightTransform()); 

    omniShadowShader.Validate(); 

    omniShadowShader.RenderScene(); 

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // attach the default buffer
}


int main(){
    mainWindow = Display(true); 
    mainWindow.Initialise(); 

    LTC_matrices mLTC;
    mLTC.mat1 = loadTexture(LTC1);
    mLTC.mat2 = loadTexture(LTC2);
    
    physicalWorld = PhysicalWorld();
    CreateShaders();
    CreateObjects(); 
    Skybox skybox = Skybox();

    camera = Camera(glm::vec3(0.0f, 15.0f, -25.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -30.0f);

    LightConstructor lightConstructor = LightConstructor();
    mainLight = lightConstructor.getMainLight();
    
    pointLights = lightConstructor.getPointLight();
    pointLightCount = lightConstructor.getPointLightCount();
    pointLightObjects = lightConstructor.getPointLightObjects();
    objectLightShader.addObjects(pointLightObjects);

    spotLights = lightConstructor.getSpotLight();
    spotLightCount = lightConstructor.getSpotLightCount();
    spotLightObjects = lightConstructor.getSpotLightObjects();
    objectLightShader.addObjects(spotLightObjects);

    areaLights = lightConstructor.getAreaLight();
    areaLightCount = lightConstructor.getAreaLightCount();
    areaLightObjects = lightConstructor.getAreaLightObjects();
    objectLightShader.addObjects(areaLightObjects);
    

	glfwSwapInterval(1);

    Process process = Process(&mainWindow, &camera, &physicalWorld, &objectShader, &directionalShadowShader, &omniShadowShader, &bumpMapShader, &paralaxMapShader);
    glfwSetWindowUserPointer(mainWindow.getWindow(), reinterpret_cast<void *>(&camera));
	process.initMousePosition();
    
	GUI gui(&process, &mainWindow, &physicalWorld, &objectShader, &directionalShadowShader, &omniShadowShader, &bumpMapShader, &paralaxMapShader, &objectLightShader);
    
    camera.processKeyboardMovement(LEFT, 0.1); // DO NOT REMOVE ! Fix bug updating camera position
    while(!mainWindow.getShouldClose()){
        Time::updateTime();
        process.processInput();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.1, 200.0);
        // we want the perspective projection instead of orthographic (made for 2D)
        // 1st: Field of view : how wide the view is (top to bottom) , 
        //2scd : width of the window/height, 3rd; Near plane: how near can you see an object (we don't want to see through it)
        // 4th Far plane: everything beyond this plane is cut off. 

         // Get and Handle user input
         
        glfwPollEvents(); 

        physicalWorld.animate();
        
        // Order is really important : order = shadow, object, objectLight
        directionalShadowShader.DirectionalShadowMapPass(mainLight); // shadow map will be updated for the light passed 
        for(size_t i = 0; i < pointLightCount;  i++ ){
            OmniShadowMapPass(&pointLights[i]); 
        }
        for (size_t i = 0; i < spotLightCount; i++){
            OmniShadowMapPass(&spotLights[i]); 
        }

        mainWindow.resetViewport();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.DrawSkyBox(view, projection);

        glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, mLTC.mat1);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, mLTC.mat2);

        bumpMapShader.RenderBump(camera, projection, view, &lightConstructor); 
        paralaxMapShader.RenderParalax(camera, projection, view, &lightConstructor); 
        
        objectShader.RenderPass(camera, projection, view, &lightConstructor); 
        objectLightShader.DrawLightObjects(projection, view);
        shader2D.drawObject();
             
        gui.update();
        mainWindow.swapBuffers(); // There are 2 scenes going on at once, we are drawing to the one that can't be seen, and we call swapBuffers to swap them around: so then the one we are drawing to is the one that can be seen and the one which could be seen originaly is the one we are drawing to. 
    }

    gui.clear();

    return 0; 
}

