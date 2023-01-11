
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

Display mainWindow; 

PhysicalWorld physicalWorld;

Shader objectShader; 
Shader directionalShadowShader; 
Shader areaLightShader; 

Camera camera; 

DirectionalLight* mainLight; 
PointLight *pointLights; 
SpotLight *spotLights; 
AreaLight areaLights[values::MAX_AREA_LIGHTS]; 

int pointLightCount; 
int spotLightCount;
int areaLightCount = 0;

void CreateObjects(){

    Object* ground = new Object(geometry::plane, Textures::Wood(), Materials::Dull(), glm::vec3(0., 0., 0.), glm::vec3(0.), glm::vec3(general::sceneSize.x/2., general::floorThickness, general::sceneSize.z/2), 1, glm::vec3(1., 1., 1.));
    physicalWorld = PhysicalWorld(ground);
    objectShader.addObject(ground);
    directionalShadowShader.addObject(ground); 

    for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(), 2.+5*i, Utils::getRandom());
		glm::vec3 rot = glm::vec3(Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(Utils::getRandom(0.5,2.));
		glm::vec3 color = glm::vec3(1.0);
		Object* sphere = new Object(geometry::sphere, Textures::Dirt(), Materials::Shiny(), pos, rot, scale, 1, color);
        objectShader.addObject(sphere);
        directionalShadowShader.addObject(sphere);
        physicalWorld.addSphere(sphere); 
	}

	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(), 2.+5*i, Utils::getRandom());
		glm::vec3 rot = glm::vec3(Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(Utils::getRandom(0.5,2.), Utils::getRandom(0.5,2.), Utils::getRandom(0.5,2.));
		glm::vec3 color = glm::vec3(1.0);
		Object* cube = new Object(geometry::cube, Textures::Brick(), Materials::Shiny(), pos, rot, scale, 1, color);
        objectShader.addObject(cube);
        directionalShadowShader.addObject(cube);
        physicalWorld.addCube(cube); 
	}
}

void CreateShaders()
{
    objectShader.CreateFromFiles(shaderfiles::mainVertex, shaderfiles::mainFrag); 
    directionalShadowShader.CreateFromFiles(shaderfiles::shadowMapVertex, shaderfiles::shadowMapFrag); 
    areaLightShader.CreateFromFiles(shaderfiles::lightPlaneVertex, shaderfiles::lightPlaneFrag); 
}

int main(){

    mainWindow = Display(true); 
    mainWindow.Initialise(); 
    
    CreateShaders();
    CreateObjects(); 

    camera = Camera(glm::vec3(0.0f, 15.0f, -25.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -30.0f);

    LightConstructor lightConstructor = LightConstructor();
    mainLight = lightConstructor.getMainLight();
    pointLights = lightConstructor.getPointLight();
    pointLightCount = lightConstructor.getPointLightCount();
    spotLights = lightConstructor.getSpotLight();
    spotLightCount = lightConstructor.getSpotLightCount();
    
	for (int i=0; i<1; i++) {
		glm::vec3 pos = glm::vec3(-10,1,0);
		glm::vec3 rot = glm::vec3(glm::radians(-90.0), 0, 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		glm::vec3 scale = glm::vec3(1);

		Object* plane = new Object(geometry::plane, Textures::Dirt(), Materials::Empty(), pos, rot, scale, 1, glm::vec3(1,0,0));
		areaLightShader.addObject(plane);

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
    while(!mainWindow.getShouldClose()){
        Time::updateTime();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.001, 200.0);
        // we want the perspective projection instead of orthographic (made for 2D)
        // 1st: Field of view : how wide the view is (top to bottom) , 
        //2scd : width of the window/height, 3rd; Near plane: how near can you see an object (we don't want to see through it)
        // 4th Far plane: everything beyond this plane is cut off. 

         // Get and Handle user input
         
        glfwPollEvents(); 

        physicalWorld.animate();
        
        // Order is really important : order = shadow, object, objectLight
        directionalShadowShader.DirectionalShadowMapPass(mainLight); // shadow map will be updated for the light passed 
        objectShader.RenderPass(camera, projection, view, mainLight, pointLights, pointLightCount, spotLights, spotLightCount, areaLights, areaLightCount); 
        areaLightShader.DrawLightObjects(projection, view);
        
             
        gui.update();
        process.processInput();
        mainWindow.swapBuffers(); // There are 2 scenes going on at once, we are drawing to the one that can't be seen, and we call swapBuffers to swap them around: so then the one we are drawing to is the one that can be seen and the one which could be seen originaly is the one we are drawing to. 
    }

    gui.clear();

    return 0; 
}

