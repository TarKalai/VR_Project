#pragma once
#include "pointLight.h"

class SpotLight: 
    public PointLight
{
public : 
    SpotLight();

    SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat xDir, GLfloat yDir, GLfloat zDir, 
        GLfloat con, GLfloat lin, GLfloat exp,
        GLfloat edg); 
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
                GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, 
                GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, 
                GLuint edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir); // set the spotlight to the direction and position passed, we can pass the camera position and the camera front direction


    ~SpotLight(); 

private: 
    glm::vec3 direction; // direction the spotlight is flashing 

    GLfloat edge; // edge of the light : how much is not going to be shown on 
    GLfloat procEdge; // will be the cos value of edge. 

};