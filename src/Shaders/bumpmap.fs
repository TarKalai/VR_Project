#version 330 core

out vec4 color;

in vec2 TexCoord; 
in vec3 FragPos;
in mat3 TBN;
in vec4 DirectionalLightSpacePos; 

struct Light
{
    vec3 color; 
    float ambientIntensity; 
    float diffuseIntensity; 
};

struct DirectionalLight{
    Light base; 
    vec3 direction; 
}; 

struct Material{
    float specularIntensity; 
    float shininess;  
    sampler2D diffuse;
    vec4 albedoRoughness; // (x,y,z) = color, w = roughness
};

uniform sampler2D theTexture;
uniform sampler2D normalMap;
uniform sampler2D directionalShadowMap; 


uniform DirectionalLight directionalLight;


uniform Material material; 


uniform vec3 eyePosition;
uniform vec3 objectColor;

float CalcDirectionalShadowFactor(DirectionalLight light)
{
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w; 
    projCoords = (projCoords * 0.5) + 0.5; 
    float current = projCoords.z; 
    vec3 normal = texture(normalMap, TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 lightDir= normalize(light.direction); 
    float bias = min(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);
    float shadow = 0.0; 
    vec2 texelSize = 1.0/textureSize(directionalShadowMap, 0); 
    
    for(int x = -1; x <= 1; ++x ){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r; 
            shadow += current - bias > pcfDepth ? 1.0 : 0.0; 
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0){
        shadow = 0.0; 
    }
    return shadow; 
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) // in the point light calculation there is a part where we need to compute the light based on the direction, directional light calculate all of it by direction. 
{
    vec3 TangentLightPos = TBN * (-direction);
    vec3 TangentViewPos  = TBN * eyePosition;
    vec3 TangentFragPos  = TBN * FragPos;    
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, TexCoord).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    //vec3 fragColor = texture(theTexture, TexCoord).rgb;
    // ambient
    //vec3 ambient = 0.1 * fragColor;
    // diffuse
    //vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    //float diff = max(dot(lightDir, normal), 0.0);
    //vec3 diffuse = diff * fragColor;
    // specular
    //vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    //vec3 reflectDir = reflect(-lightDir, normal);
    //vec3 halfwayDir = normalize(lightDir + viewDir);  
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    //vec3 specular = vec3(0.2) * spec;
    //color = vec4(ambient + diffuse + specular, 1.0);//vec4(ambient + diffuse + specular, 1.0);
    
    
    
    vec4 ambientColor = vec4(light.color, 1.0f)*light.ambientIntensity; // convert the color into vec4
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diffuseFactor = max(dot(normalize(normal), normalize(lightDir)), 0.0); 

    // 0 if it is neg. 
    // By taking the norm we have length of 1 for the magnitude of the vectors, so the result of 
    // dot product is: dot(A, B) = |A||B|cos(angle) where |A| = 1 = |B|, we effectivly get the angle. 

    vec4 diffuseColor = vec4(light.color, 1.0f)*light.diffuseIntensity * diffuseFactor; // diffuseFactor check if the light is at an angle that can be allowed to appear. 
    
    vec4 specularColor = vec4(0,0,0,0); 

    if(diffuseFactor>0.0f)
    {

        vec3 fragToEye = normalize(TangentViewPos - TangentFragPos); // we just want the direction where the fragment is from the eye
        vec3 reflectedVertex = normalize(lightDir + fragToEye);  // we want ot know where the light ray is reflected around the normal. The first argument is what we want ot reflect and the scd is what we are reflecting it around. 
        // we are reflecting the light direction across the normal pointing out of the object. 
        // if fragToEye is the same as reflectedVertex then we will see bright light because specular is most intance at that point. 

        float specularFactor = dot(normal, reflectedVertex); 

        if (specularFactor >0.0){
            specularFactor = pow(specularFactor, material.shininess); //We update the specular factor
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);  
        }
    }

    return (ambientColor +  (1.0 -  shadowFactor) * (diffuseColor + specularColor));

}


vec4 CalcDirectionalLight()
{   
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight); 
    return CalcLightByDirection(directionalLight.base, directionalLight.direction*1000, shadowFactor); 
}

void main()
{       
    vec4 finalColor = CalcDirectionalLight();
    color = texture(theTexture, TexCoord)*finalColor*vec4(objectColor, 1.0);

}