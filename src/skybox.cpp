#include "skybox.h"

Skybox::Skybox(){}

Skybox::Skybox(std::vector<std::string> faceLocations, bool day){

    // Shader Setup
    skyShader = new Shader(); 
    skyShader->CreateFromFiles("../../src/Shaders/skybox.vs", "../../src/Shaders/skybox.fs"); 

    uniformProjection = skyShader->GetProjectionLocation(); 
    uniformView = skyShader->GetViewLocation(); 
    // uniformFogColor = skyShader->GetFogColorLocation(); 

    // Texture Setup
    // glGenTextures(1, &textureId); 
    // glBindTexture(GL_TEXTURE_CUBE_MAP, textureId); // generate the texture and bind it to thz cubemap.

    if (day) {
        glGenTextures(1, &textureDay); 
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureDay);
    } else {
        glGenTextures(1, &textureNight); 
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureNight);
    }

    int width, height, bitDepth; 

    for(size_t i = 0; i < 6; i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);

        if(!texData){
            printf("Failed to find %s\n", faceLocations[i].c_str()); 
            return; 
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        //GL_RGB no alpha channel because we don't really expect to see trough the skybox
        stbi_image_free(texData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // GL_TEXTURE_WRAP_S: it defines how the texture behaves around the s axis which corresponds to the x axis, with GL_REPEAT we tell it to trepeat the texture once we go over the border. 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // With GL_LINEAR it will blend the pixels together (the other type: GL_NEAREST we will have a more pixalated look) 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_TEXTURE_MAG_FILTER: when we are closer to the image. 


    skyMesh = new Object(geometry::cube, NULL, Materials::Empty(), glm::vec3(0.), glm::vec3(1.), glm::vec3(1.), false, glm::vec3(1.));
    skyMesh->MakeObject(); 

}

// void Skybox::LoadFogColor(float r, float g, float b)
// { 
//     skyShader->SetFogColor(r, g, b); 
// }

void Skybox::BindTextures()
{
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureDay); 
    glActiveTexture(GL_TEXTURE2); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureNight); 
    skyShader->SetBlendFactor(0.5f); // blend = 0.5 equal mix of day and night textures 

}

void Skybox::DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{

    // we don't want to take into account the translation part of the camera => we get rid of the last column of the transformation matrix
    // which deal with translations
    
    viewMatrix = glm::mat3(viewMatrix); 
    rotation += 0.05; 
    if(rotation > 360.0f)
    {
        rotation = 0.1f; 
    }
    viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation), glm::vec3(0., 1., 0.)); 
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));
    // first we convert into 3x3 matrix then to a 4x4 matrix , we will get a blank 4th col.

    glDepthMask(GL_FALSE); // so that the sky box is infinitely far away, no depth perception

    skyShader->UseShader(); 
    BindTextures();
    skyShader->ConnectSkyboxes();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    // LoadFogColor(0.5, 0.5, 0.5); 

    // glActiveTexture(GL_TEXTURE0); // we can do GL_TEXTURE0 because this shader is used independently from the other shaders 
    // // the cube map is set to texture0

    // glBindTexture(GL_TEXTURE_CUBE_MAP, textureId); 

     


    // we don't need to set the uniforms because by default it is set to 0

    skyShader->Validate(); 

    skyMesh->draw(); 

    glDepthMask(GL_TRUE); 
}

Skybox::~Skybox(){}