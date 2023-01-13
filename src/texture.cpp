
#include "texture.h"
#include <iostream>


Texture::Texture(){
    name = "";
    textureID = 0; 
    width = 0; 
    height = 0; 
    nrComponents=0;
    fileLocation="";
    LoadTexture();
}

Texture::Texture(const char* fileLoc, const char* _name){
    name = _name;
    textureID = 0; 
    width = 0; 
    height = 0; 
    nrComponents=0;
    fileLocation=fileLoc; 
    LoadTexture();
}

bool Texture::LoadTexture(){
    std::cout << fileLocation << std::endl;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(fileLocation, &width, &height, &nrComponents, 0);

    if (!data) {
        printf("Failed to find texture : ", fileLocation); 
        return false; 
    }

    GLenum dataFormat;
    if (nrComponents == 1) {
        dataFormat = GL_RED;
    } else if (nrComponents == 3) { // internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
        dataFormat = GL_RGB;
    } else if (nrComponents == 4) { // internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
        dataFormat = GL_RGBA;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); // The 3D type of texture: for instance if w efly through a could, there is texture inside the cloud. 
    //3. Define the parameters for the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // GL_TEXTURE_WRAP_S: it defines how the texture behaves around the s axis which corresponds to the x axis, with GL_REPEAT we tell it to trepeat the texture once we go over the border. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // The S and T axis corresponds to the u,v coordinates. (x and y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // With GL_LINEAR it will blend the pixels together (the other type: GL_NEAREST we will have a more pixalated look) 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_TEXTURE_MAG_FILTER: when we are closer to the image. 
    
    glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // generate MipMap automatically. 

    if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic"))
    {
        GLfloat value, max_anisotropy = 16.0f; /* don't exceed this value...*/
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, & value);

        value = (value > max_anisotropy) ? max_anisotropy : value; 
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
    }
    else
    {
        printf("Anisotropic Filtering is not supported."); 
        return false; 
    }

    glBindTexture(GL_TEXTURE_2D, 0); 
    stbi_image_free(data);
    return true;
}


void Texture::UseTexture(){
    glActiveTexture(GL_TEXTURE0); // GL_TEXTURE0: texture unit => when the texture is run in the fragment shader, there will be a sampler that will have access to the data for the texture and it accesses it throught the texture unit. 
    glBindTexture(GL_TEXTURE_2D, textureID); // bind to the texture unit. 

}


void Texture::ClearTexture() {
    glDeleteTextures(1, &textureID); 
    textureID = 0; 
    width=0; 
    height=0; 
    nrComponents=0;
    fileLocation=""; 
}

Texture::~Texture(){
    ClearTexture(); 
}