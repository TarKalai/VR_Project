#version 330

layout (location = 0) in vec3 pos; 

uniform float time; 

void main()
{
    vec3 position = pos; 
    position.y = sin(time + pos.y); 
    gl_Position = vec4(position, 1.0); 
}