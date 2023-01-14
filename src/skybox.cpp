#include "skybox.h"

Skybox::Skybox(){
    createShader();
    setDay("lake");
    setNight("temple");
}

void Skybox::setDay(std::string name) {
    std::vector<std::string> skyboxFacesDay;
    skyboxFacesDay.push_back("../../image/Skyboxes/"+name+"_posx.jpg"); 
    skyboxFacesDay.push_back("../../image/Skyboxes/"+name+"_negx.jpg"); 
    skyboxFacesDay.push_back("../../image/Skyboxes/"+name+"_posy.jpg"); 
    skyboxFacesDay.push_back("../../image/Skyboxes/"+name+"_negy.jpg"); 
    skyboxFacesDay.push_back("../../image/Skyboxes/"+name+"_posz.jpg"); 
    skyboxFacesDay.push_back("../../image/Skyboxes/"+name+"_negz.jpg");
    // DAY
    glGenTextures(1, &textureDay); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureDay);

    linkTexture(skyboxFacesDay); 
}

void Skybox::setNight(std::string name) { 
    std::vector<std::string> skyboxFacesNight; 

    skyboxFacesNight.push_back("../../image/Skyboxes/"+name+"_posx.jpg"); 
    skyboxFacesNight.push_back("../../image/Skyboxes/"+name+"_negx.jpg"); 
    skyboxFacesNight.push_back("../../image/Skyboxes/"+name+"_posy.jpg"); 
    skyboxFacesNight.push_back("../../image/Skyboxes/"+name+"_negy.jpg"); 
    skyboxFacesNight.push_back("../../image/Skyboxes/"+name+"_posz.jpg"); 
    skyboxFacesNight.push_back("../../image/Skyboxes/"+name+"_negz.jpg");
    // NIGHT
    glGenTextures(1, &textureNight); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureNight);
    linkTexture(skyboxFacesNight);
}

void Skybox::createShader() {
    skyShader = new Shader(); 
    skyShader->CreateFromFiles("../../src/Shaders/skybox.vs", "../../src/Shaders/skybox.fs"); 

    uniformProjection = skyShader->GetProjectionLocation(); 
    uniformView = skyShader->GetViewLocation(); 
}

void Skybox::linkTexture(std::vector<std::string> faceLocations) {
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


    skyMesh = new Object(geometry::cube, NULL, Materials::Empty(), glm::vec3(0.), glm::vec3(1.), glm::vec3(1.), glm::vec3(1.));
    skyMesh->MakeObject(); 
}

void Skybox::BindTextures()
{
    glActiveTexture(GL_TEXTURE11); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureDay); 
    glActiveTexture(GL_TEXTURE12); 
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

    skyShader->Validate(); 
    skyMesh->draw(); 
    glDepthMask(GL_TRUE); 
}

Skybox::~Skybox(){}