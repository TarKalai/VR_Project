#pragma once


#include "light.h"
#include <vector>

class AreaLight :

    public Light
{
public: 
    AreaLight(); 

    AreaLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        glm::vec3 Position, 
        GLfloat con, GLfloat lin, GLfloat exp, 
        glm::vec3 Rotation, bool TwoSided, std::vector<glm::vec3> VertexPosition, glm::vec3 Scale); 

    void UseLight(GLuint uniformPoint0, GLuint uniformPoint1, 
                GLuint uniformPoint2, GLuint uniformPoint3,
                GLuint uniformColor, GLuint uniformIntensity, GLuint uniformTwoSided);

    glm::mat4 getModelRotPos(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    

    std::vector<glm::vec3>getVertexPosition(){return vertexPosition;}
    glm::vec3 getPosition(){return position;}
    glm::vec3 getRotation(){return rotation;}
    glm::vec3 getScale(){return scale;}
    bool getTwoSided(){return twoSided;}
 
    ~AreaLight(); 

protected: 
    glm::vec3 position; // where the light is coming from
    glm::vec3 rotation;
    glm::vec3 scale;
    GLfloat constant, linear, exponent; //ax² + bx + c; cosntant = c, linear = b, exponent =a; 
    bool twoSided;
    std::vector<glm::vec3> vertexPosition;
};