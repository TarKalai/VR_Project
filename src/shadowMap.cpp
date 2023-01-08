
#include "shadowMap.h"

ShadowMap::ShadowMap(){

    FBO = 0; 
    shadowMap = 0; 
}

bool ShadowMap::Init(GLuint width, GLuint height){

    shadowWidth = width; 
    shadowHeight = height; 

    // generate FBO => it can be considered as a screen (with glSwap, we are swapping between the different buffers)

    glGenFramebuffers(1, &FBO);
    
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); // arg2: MipMap level, 
    //arg3: depth value based on the far plane => if far plane is 100, and the pixel is at 50 then this will be set at 0.5 because it is halfway
    // last arg: we don't need to precise because shadowMap is binded to the FBO and the FBO will output the image to the texture (we keep updating it)
    // this is why we don't pass anything to it because we don't actually have anything to pass to it.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    // With GL_LINEAR it will blend the pixels together (the other type: GL_NEAREST we will have a more pixalated look)
    // we are using GL_NEAREST for the shadow 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    // GL_TEXTURE_MAG_FILTER: when we are closer to the image. 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//Can't have GL_REPEAT with shadows, otherwise OVERSAMPLING: the shadows will be repeated  
    // GL_CLAMP_TO_BORDER: beyond the edge of the texture there is a single color that expands on infinitely
    // if we set the border to be 1 then whatever the camera manages to see, we garantee there is going to be light beyond the border. 
    // GL_TEXTURE_WRAP_S: it defines how the texture behaves around the s axis which corresponds to the x axis, with GL_REPEAT we tell it to trepeat the texture once we go over the border. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // The S and T axis corresponds to the u,v coordinates. (x and y)
    
    float bColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // alll values are set to 1 so it is as far away from the lightpos as possible 
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);// we are setting the border color
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0); // going to connect the frame buffer to the texture
    // GL_DEPTH_ATTACHMENT : means it is going to use the depth values when it is writing to the depth map. 
    // GL_TEXTURE_2D : type of the target
    // shadowMap : id of the texture to write to 
    //  0 : Mip-map level

    // we don't want to draw the color, so we need to state it: 
    glDrawBuffer(GL_NONE); 
    glReadBuffer(GL_NONE); 

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); // get if there are errors 

    if (status != GL_FRAMEBUFFER_COMPLETE){ 

        printf("Framebuffer Error: %i\n", status); 
        return false; 

    }    
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // set to default frabe buffer the one behind the screen

    return true; 

}

void ShadowMap::Write(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // instead to draw to the default buffer, it is drawing to the one off screen
}

void ShadowMap::Read(GLenum textureUnit){
    glActiveTexture(textureUnit); // read from the shadowMap
    glBindTexture(GL_TEXTURE_2D, shadowMap); 

}

ShadowMap::~ShadowMap(){

    if(FBO)

    {
        glDeleteFramebuffers(1, &FBO); 
    }

    if (shadowMap)
    {
        glDeleteTextures(1, &shadowMap); 

    }

}