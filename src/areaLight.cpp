#include "areaLight.h"

AreaLight::AreaLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f); 
    constant = 1.0f; 
    linear = 0.0f; 
    exponent = 0.0f; 
    rotation = glm::vec3(0.0);
    twoSided = true;
    
}

AreaLight::AreaLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        glm::vec3 Position, 
        GLfloat con, GLfloat lin, GLfloat exp,
        glm::vec3 Rotation, bool TwoSided, std::vector<glm::vec3> VertexPosition, glm::vec3 Scale) : Light(red, green, blue, aIntensity, dIntensity)
{
    position = Position; 
    constant = con; 
    linear = lin; 
    exponent = exp; 
    rotation = Rotation;
    twoSided = TwoSided;
    vertexPosition = VertexPosition;
    scale = Scale;
}
        
void AreaLight::UseLight(GLuint uniformPoint0, GLuint uniformPoint1, 
                GLuint uniformPoint2, GLuint uniformPoint3,
                GLuint uniformColor, GLuint uniformIntensity, GLuint uniformTwoSided)
{
    glm::mat4 model = getModelRotPos(position, rotation, scale);
    glm::vec3 p0 = glm::vec3(model * glm::vec4(vertexPosition[3], 1.0f));
    glm::vec3 p1 = glm::vec3(model * glm::vec4(vertexPosition[1], 1.0f));
    glm::vec3 p2 = glm::vec3(model * glm::vec4(vertexPosition[0], 1.0f));
    glm::vec3 p3 = glm::vec3(model * glm::vec4(vertexPosition[2], 1.0f));

    glUniform3fv(uniformPoint0, 1, &p0[0]);
    glUniform3fv(uniformPoint1, 1, &p1[0]);
    glUniform3fv(uniformPoint2, 1, &p2[0]);
    glUniform3fv(uniformPoint3, 1, &p3[0]);
    glUniform3fv(uniformColor, 1, &color[0]);
    glUniform1f(uniformIntensity, diffuseIntensity);
    glUniform1i(uniformTwoSided, twoSided);
}

glm::mat4 AreaLight::getModelRotPos(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    float x1 = (cos(rotation.y)*cos(rotation.z)) * scale.x; 
    float x2 = (sin(rotation.x)*sin(rotation.y)*cos(rotation.z) - cos(rotation.x)*sin(rotation.z)) * scale.y;
    float x3 = (cos(rotation.x)*sin(rotation.y)*cos(rotation.z) + sin(rotation.x)*sin(rotation.z)) * scale.z;
    float y1 = (cos(rotation.y)*sin(rotation.z)) * scale.x;
    float y2 = (sin(rotation.x)*sin(rotation.y)*sin(rotation.z) + cos(rotation.x)*cos(rotation.z)) * scale.y;
    float y3 = (cos(rotation.x)*sin(rotation.y)*sin(rotation.z) - sin(rotation.x)*cos(rotation.z)) * scale.z;
    float z1 = (-sin(rotation.y)) * scale.x;
    float z2 = (sin(rotation.x)*cos(rotation.y)) * scale.y;
    float z3 = (cos(rotation.x)*cos(rotation.y)) * scale.z;
    glm::mat4 model = glm::mat4(x1,y1,z1,0,x2,y2,z2,0,x3,y3,z3,0,position.x,position.y,position.z,1);
    return model;
}


AreaLight::~AreaLight(){}