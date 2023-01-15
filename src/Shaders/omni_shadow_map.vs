#version 330
// we take each point and pass it across. 
layout (location = 0) in vec3 pos; 

uniform mat4 model; 

void main()
{
    gl_Position = model * vec4(pos, 1.0);
}
