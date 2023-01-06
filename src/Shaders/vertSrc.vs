
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
    vertexColor = vec4(pos, 1.0);
    TexCoord = tex;
    Normal = mat3(transpose(inverse(model))) * norm; 
    FragPos =  (model * vec4(pos, 1.0)).xyz;
}

















//#version 330
//in vec3 pos;
//in vec3 norm; // normal of the vertex we are looking at, it as a direction
// we will define the normalas as an average of each vertex. 
/*
out vec4 vertexColor;
out vec3 Normal; 
out vec3 FragPos; 

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 
void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertexColor = vec4(clamp(pos, 0.0, 1.0), 1.0);

    Normal = mat3(transpose(inverse(model))) * norm; 
    // Normal is in relation to where the model is, because if we move the light around we are lihghting objects in different ways. 
    // If we rotate the object the normal doe not move, or if we scale an object the normal will bend so we need to multiply norm by 
    // the transform of the model. 
    // if we scale in only one direction it will also create some problems: the normal will start to point upwards
    // so if we scale the object uniformly the normals stay in the same direction but if we scale in one dir. the normals start to change dir. 
    // to solve this we take the inverse and take the transpose => invert scaling process

    FragPos =  (model * vec4(pos, 1.0)).xyz; // we only need to know where it is in the world for the light. 
    // we want vec3 not vec4 we take .xyz
}
*/
















//#version 330 core

//in vec3 position;
//in vec3 normal;

//out vec4 vertexColor; 

//uniform mat4 M;
//uniform mat4 V;
//uniform mat4 P;


//void main(){
//    gl_Position = P*V*M*vec4(position, 1.0);
//    vertexColor = vec4(clamp(position, 0.0, 1.0)*normal, 1.0);
//}