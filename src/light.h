#include <glm/glm.hpp>
#include <glad/glad.h>

class Light{

public: 
    Light(); 
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity); 

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation); 

    ~Light(); 

private: 

    glm::vec3 color; 
    GLfloat ambientIntensity;
}; 