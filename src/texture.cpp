
#include "texture.h"


Texture::Texture(){

    textureID = 0; 
    width = 0; 
    height = 0; 
    bitDepth=0; 
    fileLocation="";
}

Texture::Texture(const char* fileLoc){

    textureID = 0; 
    width = 0; 
    height = 0; 
    bitDepth=0; 
    fileLocation=fileLoc; 

}

bool Texture::LoadTexture(){

    stbi_set_flip_vertically_on_load(true);
    unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

    if(!texData){
        printf("Failed to find %s\n", fileLocation); 
        return false; 
    }

    glGenTextures(1, &textureID); 
    glBindTexture(GL_TEXTURE_2D, textureID); // The 3D type of texture: for instance if w efly through a could, there is texture inside the cloud. 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // GL_TEXTURE_WRAP_S: it defines how the texture behaves around the s axis which corresponds to the x axis, with GL_REPEAT we tell it to trepeat the texture once we go over the border. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // The S and T axis corresponds to the u,v coordinates. (x and y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // With GL_LINEAR it will blend the pixels together (the other type: GL_NEAREST we will have a more pixalated look) 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_TEXTURE_MAG_FILTER: when we are closer to the image. 

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);  // generate MipMap automatically. 

    glBindTexture(GL_TEXTURE_2D, 0); 

    stbi_image_free(texData);

    return true;  
}

bool Texture::LoadTextureA(){

    stbi_set_flip_vertically_on_load(true);

    unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

    if(!texData){
        printf("Failed to find %s\n", fileLocation); 
        return false; 
    }

    glGenTextures(1, &textureID); 
    glBindTexture(GL_TEXTURE_2D, textureID); // The 3D type of texture: for instance if w efly through a could, there is texture inside the cloud. 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // GL_TEXTURE_WRAP_S: it defines how the texture behaves around the s axis which corresponds to the x axis, with GL_REPEAT we tell it to trepeat the texture once we go over the border. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // The S and T axis corresponds to the u,v coordinates. (x and y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // With GL_LINEAR it will blend the pixels together (the other type: GL_NEAREST we will have a more pixalated look) 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_TEXTURE_MAG_FILTER: when we are closer to the image. 

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);  // generate MipMap automatically. 

    glBindTexture(GL_TEXTURE_2D, 0); 

    stbi_image_free(texData);

    return true;  
}


void Texture::UseTexture(){
    
    glActiveTexture(GL_TEXTURE0); // GL_TEXTURE0: texture unit => when the texture is run in the fragment shader, there will be a sampler that will have access to the data for the texture and it accesses it throught the texture unit. 

    glBindTexture(GL_TEXTURE_2D, textureID); // bind to the texture unit. 

}


void Texture::ClearTexture(){

    glDeleteTextures(1, &textureID); 
    textureID = 0; 
    width=0; 
    height=0; 
    bitDepth=0; 
    fileLocation=""; 
}

Texture::~Texture(){
    ClearTexture(); 
}