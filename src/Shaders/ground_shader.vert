#version 330 core

in vec3 pos; // position
in vec3 norm; //Normal
in vec2 tex;

out vec2 TexCoord;
out vec4 vertexColor;
out vec3 Normal; 
out vec3 FragPos; 
out vec4 DirectionalLightSpacePos; // position of the fragment is relative to the light

uniform mat4 directionalLightTransform; //to know where in space the fragment is relative to the light and the camera => the camera will tell where it is to the light and light will check if it is closer on the shadowMap or not
uniform mat4 model; // M
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);
    DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0); // model * vec(pos, 1.0) : point that the camera can see, and light may not see, and here we are saying where it is relative to the light
    vertexColor = vec4(clamp(pos, 0.0, 1.0), 1.0);
    TexCoord = tex;
    Normal = mat3(transpose(inverse(model))) * norm; 
    FragPos =  (model * vec4(pos, 1.0)).xyz;
}