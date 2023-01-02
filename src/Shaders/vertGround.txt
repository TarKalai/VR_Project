#version 330 core

in vec3 pos; // position
in vec3 norm; //Normal
in vec2 tex;

out vec2 TexCoord;
out vec4 vertexColor;
out vec3 Normal; 
out vec3 FragPos; 

uniform mat4 model; // M
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertexColor = vec4(clamp(pos, 0.0, 1.0), 1.0);
    TexCoord = tex;
    Normal = mat3(transpose(inverse(model))) * norm; 
    FragPos =  (model * vec4(pos, 1.0)).xyz;
}