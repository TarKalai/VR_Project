
#version 330 core
layout (location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 

void main ()
{
	gl_Position = projection * view * model * vec4(pos,1.0);
	// gl_Position = projection* view * vec4(pos,1.0);
}

// #version 330

// layout (location = 0) in vec3 pos; 

// // uniform float time; 

// void main()
// {
//     // vec3 position = pos; 
//     // position.y = sin(0.5 + pos.y); 
//     gl_Position = vec4(pos, 1.0); 
// }