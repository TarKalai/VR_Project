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

#include "time.h"

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

PhysicalWorld physicalWorld;
Shader directionalShadowShader, omniShadowShader, objectLightShader; 
Shader objectShader, bumpMapShader, paralaxMapShader;
Shader2D shader2D;

Camera camera;
glm::mat4 view;
glm::mat4 projection;

struct LTC_matrices {
	GLuint mat1;
	GLuint mat2;
};
GLuint loadTexture(const float *ltc);
void loadLTC();

void addToShaders(Object* obj);
void CreateObjects();
void CreateShaders();


int main(){
    srand(time(0)); // For random light generation
    Display mainWindow(true); 
    mainWindow.Initialise(); 

    physicalWorld = PhysicalWorld();
    CreateShaders();
    CreateObjects(); 
    LightConstructor lightConstructor = LightConstructor();
    objectLightShader.addObjects(lightConstructor.getLightObjects());
    Skybox skybox = Skybox();
    loadLTC();

    Camera camera(glm::vec3(0.0f, 15.0f, -25.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -30.0f);

    Process process(&mainWindow, &camera, &physicalWorld, &objectShader, &directionalShadowShader, &omniShadowShader, &bumpMapShader, &paralaxMapShader);
    GUI gui(&process, &mainWindow, &physicalWorld, &objectShader, &directionalShadowShader, &omniShadowShader, &bumpMapShader, &paralaxMapShader, &objectLightShader, &lightConstructor);
	process.initMousePosition();

    glfwSetWindowUserPointer(mainWindow.getWindow(), reinterpret_cast<void *>(&camera));
    camera.processKeyboardMovement(LEFT, 0.1); // DO NOT REMOVE ! Fix bug updating camera position
    

    glfwSwapInterval(1);
    while(!mainWindow.getShouldClose()){
        Time::updateTime();
        process.processInput();

        view = camera.getViewMatrix();
        projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.1, 200.0);
         
        glfwPollEvents(); 

        physicalWorld.animate();
        
        // Order is really important : order = shadow, object, objectLight
        directionalShadowShader.DirectionalShadowMapPass(&lightConstructor); 
        omniShadowShader.OmniShadowMapPass(&lightConstructor); 

        mainWindow.resetViewport();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        skybox.DrawSkyBox(view, projection);
        objectShader.RenderPass(camera, projection, view, &lightConstructor); 
        bumpMapShader.RenderBump(camera, projection, view, &lightConstructor); 
        paralaxMapShader.RenderParalax(camera, projection, view, &lightConstructor); 
        objectLightShader.DrawLightObjects(projection, view);
        shader2D.drawObject();
             
        gui.update();
        mainWindow.swapBuffers(); // There are 2 scenes going on at once, we are drawing to the one that can't be seen, and we call swapBuffers to swap them around: so then the one we are drawing to is the one that can be seen and the one which could be seen originaly is the one we are drawing to. 
    }

    physicalWorld.clear();
    gui.clear();

    return 0; 
}



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
    Object* ground = new Object(geometry::plane3D, ShaderType::BUMPMAP, Textures::Brickwall(), Materials::Shiny(), PHYSIC::UNMOVABLE, glm::vec3(0., 0., 0.), glm::vec3(0.), glm::vec3(general::sceneSize.x/4., general::floorThickness, general::sceneSize.z/4.), glm::vec3(1.));
    addToShaders(ground);

    // Object* ground2 = new Object(geometry::plane3D, ShaderType::PARALLAX, Textures::Brick2(), Materials::Shiny(), PHYSIC::UNMOVABLE, glm::vec3(10., 5.0, 10.), glm::vec3(0.), glm::vec3(general::sceneSize.x/5., general::floorThickness, general::sceneSize.z/5), glm::vec3(1.));
    // addToShaders(ground2);

    // Object* box = new Object(geometry::cube, ShaderType::PARALLAX, Textures::Toy(), Materials::Shiny(), PHYSIC::MOVABLE, glm::vec3(0., 8.0, 0.), glm::vec3(0.), glm::vec3(8.0), glm::vec3(1.));
    // addToShaders(box);

    // Object* bunny = new Object(geometry::bunny, ShaderType::BUMPMAP, Textures::Brickwall(), Materials::Shiny(), PHYSIC::MOVABLE, glm::vec3(0., 7.0, 0.), glm::vec3(0.), glm::vec3(5.0), glm::vec3(1.));
    // addToShaders(bunny);
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

GLuint loadTexture(const float *ltc) {
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

void loadLTC() {
    LTC_matrices mLTC;
    mLTC.mat1 = loadTexture(LTC1);
    mLTC.mat2 = loadTexture(LTC2);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mLTC.mat1);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, mLTC.mat2);
}