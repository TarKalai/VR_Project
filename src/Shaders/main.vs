#version 330
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex; 
layout (location=2) in vec3 norm; // normal of the vertex we are looking at, it as a direction
// we will define the normals as an average of each vertex. 


out vec2 TexCoord; 
out vec3 Normal; 
out vec3 FragPos;
out vec4 DirectionalLightSpacePos; // position of the fragment is relative to the light

uniform mat4 directionalLightTransform; //to know where in space the fragment is relative to the light and the camera => the camera will tell where it is to the light and light will check if it is closer on the shadowMap or not
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 


void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);
    DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0); // model * vec(pos, 1.0) : point that the camera can see, and light may not see, and here we are saying where it is relative to the light

    TexCoord = tex; 

    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalize(normalMatrix * norm); 
    // Normal is in relation to where the model is, because if we move the light around we are lihghting objects in different ways. 
    // If we rotate the object the normal doe not move, or if we scale an object the normal will bend so we need to multiply norm by 
    // the transform of the model. 
    // if we scale in only one direction it will also create some problems: the normal will start to point upwards
    // so if we scale the object uniformly the normals stay in the same direction but if we scale in one dir. the normals start to change dir. 
    // to solve this we take the inverse and take the transpose => invert scaling process

    FragPos =  (model * vec4(pos, 1.0)).xyz; // we only need to know where it is in the world for the light. 
    // we want vec3 not vec4 we take .xyz
}
