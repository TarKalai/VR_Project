#version 330 core

in vec3 pos; // position
in vec3 norm; //Normal
in vec2 tex;

uniform mat4 model; // M
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
