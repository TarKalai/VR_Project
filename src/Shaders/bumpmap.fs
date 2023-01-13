#version 330 core

out vec4 color;

in vec2 TexCoord; 
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D theTexture;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 eyePosition;

void main()
{       
    vec3 TangentLightPos = TBN * lightPos;
    vec3 TangentViewPos  = TBN * eyePosition;
    vec3 TangentFragPos  = TBN * FragPos;    
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, TexCoord).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 fragColor = texture(theTexture, TexCoord).rgb;
    // ambient
    vec3 ambient = 0.1 * fragColor;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * fragColor;
    // specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    color = vec4(ambient + diffuse + specular, 1.0);//vec4(ambient + diffuse + specular, 1.0);
}