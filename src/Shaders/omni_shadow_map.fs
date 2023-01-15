#version 330

//  We are going to overwrite the way it draws the depth component. 
in vec4 FragPos; 

uniform vec3 lightPos; 
uniform float farPlane; // how far away the light can reach. 

void main()
{
    float distance = length(FragPos.xyz - lightPos); // distance between the fragment and light position. 
    distance = distance/farPlane; // we need to have values between 0 and 1 for the depth component. 
    gl_FragDepth = distance; // we are overwritting what has been put in there in the depth buffer.

}