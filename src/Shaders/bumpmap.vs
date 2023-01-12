#version 330
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex; 
layout (location=2) in vec3 norm; 
layout (location=3) in vec3 tangent;
layout (location=4) in vec2 bitangent; 


out vec2 TexCoord; 
out vec3 FragPos;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 

uniform vec3 lightPos;
uniform vec3 eyePosition;

void main(){
    FragPos =  vec3(model * vec4(pos, 1.0)); 
    TexCoord = tex; 
    

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 N = normalize(normalMatrix * norm);
    vec3 T = normalize(normalMatrix * tangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * eyePosition;
    TangentFragPos  = TBN * FragPos;

    gl_Position = projection * view * model * vec4(pos, 1.0);
}
