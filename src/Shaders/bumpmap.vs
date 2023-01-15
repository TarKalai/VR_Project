#version 330
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex; 
layout (location=2) in vec3 norm; 
layout (location=3) in vec3 tangent;


out vec2 TexCoord; 
out vec3 FragPos;
out mat3 TBN;
out vec4 DirectionalLightSpacePos; // position of the fragment is relative to the light
out float visibility; 



uniform mat4 directionalLightTransform;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 

const float density = 0.009;
const float gradient = 5.5; 

void main(){

    vec4 positionRelativeToCam = view * model*vec4(pos, 1.0); 

    FragPos = vec3(model * vec4(pos, 1.0)); 
    DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 N = normalize(normalMatrix * norm);
    vec3 T = normalize(normalMatrix * tangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    TBN = transpose(mat3(T, B, N));  




    vec3 scale = vec3(length(normalMatrix[0]), length(normalMatrix[1]), length(normalMatrix[2]));
    TexCoord = tex*(scale.x, scale.y)/(scale.x*scale.y);  // Resize texture with object scaling 
    
    float dist = length(positionRelativeToCam.xyz);  // distance of the vertex from the camera.
    visibility = exp(-pow((dist*density), gradient)); 
    visibility = clamp(visibility, 0.0, 1.0); 

    gl_Position = projection * view * model * vec4(pos, 1.0);
}
