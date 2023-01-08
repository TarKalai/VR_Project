#version 330

// we want to know where things are in relation to the light

layout (location =0) in vec3 pos;
// in vec3 norm;
// in vec2 tex; 

uniform mat4 model; // model is going to transform the point pos into world space
uniform mat4 directionalLightTransform;// point of view of the world from the light, it is going to be a combination of the projection and view of the camera : projection * view

void main(){

    gl_Position = directionalLightTransform * model * vec4(pos, 1.0); // we are converting pos into world space relative to the light 

}