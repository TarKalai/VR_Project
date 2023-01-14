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

Display mainWindow; 

PhysicalWorld physicalWorld;

Shader objectShader; 
Shader directionalShadowShader; 
Shader objectLightShader; 
Shader2D shader2D;
Shader bumpMapShader;
Shader paralaxMapShader;

Camera camera; 

DirectionalLight* mainLight; 
PointLight *pointLights; 
SpotLight *spotLights; 
AreaLight areaLights[values::MAX_AREA_LIGHTS]; 

std::vector<Object*> pointLightObjects;

int pointLightCount; 
int spotLightCount;
int areaLightCount = 0;

void CreateObjects(){
    // GROUNDS
    Object* ground = new Object(geometry::plane, Textures::Brickwall(), Materials::Shiny(), glm::vec3(0., 0., 0.), glm::vec3(0.), glm::vec3(general::sceneSize.x/2., general::floorThickness, general::sceneSize.z/2), glm::vec3(1.), true);
    physicalWorld.addObject(ground, PHYSIC::GROUND_OBJECT);
    bumpMapShader.addObject(ground);
    directionalShadowShader.addObject(ground); 

    Object* ground2 = new Object(geometry::plane, Textures::Brickwall(), Materials::Shiny(), glm::vec3(0., 10, 0.), glm::vec3(0.), glm::vec3(general::sceneSize.x/50., general::floorThickness, general::sceneSize.z/50), glm::vec3(1.), true);
    physicalWorld.addObject(ground2, PHYSIC::GROUND_OBJECT);
    bumpMapShader.addObject(ground2);
    directionalShadowShader.addObject(ground2);


    Object* ground3 = new Object(geometry::plane, Textures::Brick2(), Materials::Shiny(), glm::vec3(10., 5.0, 10.), glm::vec3(0.), glm::vec3(general::sceneSize.x/5., general::floorThickness, general::sceneSize.z/5), glm::vec3(1.), true);
    physicalWorld.addObject(ground3, PHYSIC::GROUND_OBJECT);
    paralaxMapShader.addObject(ground3);
    directionalShadowShader.addObject(ground3);

    Object* box = new Object(geometry::cube, Textures::Toy(), Materials::Shiny(), glm::vec3(10., 10.0, 10.), glm::vec3(0.), glm::vec3(8.0), glm::vec3(1.), true);
    physicalWorld.addObject(box, PHYSIC::NORMAL_OBJECT);
    paralaxMapShader.addObject(box);
    directionalShadowShader.addObject(box);

}

void CreateShaders()
{
    objectShader.CreateFromFiles(shaderfiles::mainVertex, shaderfiles::mainFrag); 
    directionalShadowShader.CreateFromFiles(shaderfiles::shadowMapVertex, shaderfiles::shadowMapFrag); 
    objectLightShader.CreateFromFiles(shaderfiles::lightObjectVertex, shaderfiles::lightObjectFrag); 
    shader2D = Shader2D(true);
    bumpMapShader.CreateFromFiles(shaderfiles::bumpMapVertex, shaderfiles::bumpMapFrag); 
    paralaxMapShader.CreateFromFiles(shaderfiles::paralaxMapVertex, shaderfiles::paralaxMapFrag);
}

int main(){

    mainWindow = Display(true); 
    mainWindow.Initialise(); 
    
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
    

    
	for (int i=0; i<0; i++) {
		glm::vec3 pos = glm::vec3(-10,1,0);
		glm::vec3 rot = glm::vec3(glm::radians(-90.0), 0, 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		glm::vec3 scale = glm::vec3(1);

		Object* plane = new Object(geometry::plane, Textures::Dirt(), Materials::Empty(), pos, rot, scale, glm::vec3(1,0,0));
		objectLightShader.addObject(plane);

		areaLights[i] = AreaLight(plane->color.x,plane->color.y, plane->color.z, 
							  0.9f, 10.,
							  plane->getPosition(),
							  0.003f, 0.002f, 0.001f, // not used
							  plane->getRotation(), true, 
							  plane->getVertexPosition(),
							  plane->getScale());
    	areaLightCount++; 
    }

	glfwSwapInterval(1);

    Process process = Process(&mainWindow, &camera, &physicalWorld, &objectShader, &directionalShadowShader);
    glfwSetWindowUserPointer(mainWindow.getWindow(), reinterpret_cast<void *>(&camera));
	process.initMousePosition();
    
	GUI gui(&process, &mainWindow, &physicalWorld, &objectShader, &directionalShadowShader);
    
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
        mainWindow.resetViewport();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.DrawSkyBox(view, projection);

        bumpMapShader.RenderBump(camera, projection, view, mainLight, pointLights, pointLightCount, spotLights, spotLightCount); 
        paralaxMapShader.RenderParalax(camera, projection, view, mainLight, pointLights, pointLightCount, spotLights, spotLightCount); 
        
        objectShader.RenderPass(camera, projection, view, mainLight, pointLights, pointLightCount, spotLights, spotLightCount, areaLights, areaLightCount); 

        objectLightShader.DrawLightObjects(projection, view);
        shader2D.drawObject();
             
        gui.update();
        mainWindow.swapBuffers(); // There are 2 scenes going on at once, we are drawing to the one that can't be seen, and we call swapBuffers to swap them around: so then the one we are drawing to is the one that can be seen and the one which could be seen originaly is the one we are drawing to. 
    }

    gui.clear();

    return 0; 
}

