#version 330 core
out vec4 FragColour;

// uniform vec4 color;
uniform vec3 objectColor;
void main()
{

	FragColour = vec4(objectColor, 1.0);
}