#version 330 core

out vec4 color;

in vec2 TexCoord; 
in vec3 FragPos;
in mat3 TBN;
in vec4 DirectionalLightSpacePos; 
in float visibility; 

const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;
const int MAX_AREA_LIGHTS = 10;

struct Light{
    vec3 color; 
    float ambientIntensity; 
    float diffuseIntensity; 
};

struct DirectionalLight{
    Light base; 
    vec3 direction; 
}; 

struct PointLight {
    Light base; 
    vec3 position; 
    float constant;
    float linear; 
    float exponent; 
};

struct SpotLight{
    PointLight base; 
    vec3 direction; 
    float edge;
};

struct Material{
    float specularIntensity; 
    float shininess;  
    float albedoRoughness;
};

struct OmniShadowMap{
    samplerCube shadowMap;
    float farPlane; 
};

uniform int pointLightCount; 
uniform int spotLightCount; 
uniform int areaLightCount;
uniform vec3 skyColor; 

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS]; 
uniform SpotLight spotLights[MAX_SPOT_LIGHTS]; 

uniform sampler2D theTexture;
uniform sampler2D normalMap;
uniform sampler2D directionalShadowMap; 
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS]; 

uniform Material material; 

uniform vec3 eyePosition;
uniform vec3 objectColor;

vec3 sampleOffsetDirections[20] = vec3[]
(
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1), 
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), 
    vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1), 
    vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float CalcDirectionalShadowFactor(DirectionalLight light){
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

float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{   
    vec3 TangentLightPos = TBN * light.position;
    vec3 TangentViewPos  = TBN * eyePosition;
    vec3 TangentFragPos  = TBN * FragPos;    

    vec3 fragToLight = FragPos - light.position;
    float currentDepth = length(fragToLight); 
    float shadow = 0.0; 
    float bias = 0.05;
    int samples = 20; 


    float viewDistance = length(eyePosition - FragPos); 
    float diskRadius = (1.0 + (viewDistance/omniShadowMaps[shadowIndex].farPlane))/25.0; 


    for (int i = 0; i < samples; i++) {
        float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= omniShadowMaps[shadowIndex].farPlane;
        if (currentDepth - bias > closestDepth){
            shadow += 1.0; 
        } 
    }

    shadow /= float(samples); 
    return shadow; 
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) // in the point light calculation there is a part where we need to compute the light based on the direction, directional light calculate all of it by direction. 
{
    vec3 TangentLightPos = TBN * (-direction);
    vec3 TangentViewPos  = TBN * eyePosition;
    vec3 TangentFragPos  = TBN * FragPos;    

    vec3 normal = texture(normalMap, TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);  
    
    vec4 ambientColor = vec4(light.color, 1.0f)*light.ambientIntensity;
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diffuseFactor = max(dot(normalize(normal), normalize(lightDir)), 0.0); 
    vec4 diffuseColor = vec4(light.color, 1.0f)*light.diffuseIntensity * diffuseFactor;
    
    vec4 specularColor = vec4(0,0,0,0); 
    if(diffuseFactor>0.0f)
    {
        vec3 fragToEye = normalize(TangentViewPos - TangentFragPos); 
        vec3 reflectedVertex = normalize(lightDir + fragToEye);  
        float specularFactor = dot(normal, reflectedVertex); 

        if (specularFactor >0.0){
            specularFactor = pow(specularFactor, material.shininess); //We update the specular factor
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);  
        }
    }
    return (ambientColor +  (1.0 -  shadowFactor) * (diffuseColor + specularColor));
}


vec4 CalcDirectionalLight(){   
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight); 
    return CalcLightByDirection(directionalLight.base, directionalLight.direction*1000, shadowFactor); 
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
// to calculate only one of the light so that if we are doing spotLight and it needs pointLight we can use this function
{
    vec3 direction = FragPos - pLight.position; // we get the vector from the pointLight to the fragment = direction
    float distance = length(direction); 
    direction = normalize(direction);

    float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex);

    vec4 color = CalcLightByDirection(pLight.base, -pLight.position, shadowFactor); 
    float attenuation = pLight.exponent * distance * distance + pLight.linear*distance + pLight.constant;

    return (color/attenuation); // get the color of the pixel 

}

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
    
    vec3 rayDirection = normalize(FragPos - sLight.base.position); 
    // we need to know what the vector is between where we are and the fragment we are doing the calculation for
    float slFactor = dot(rayDirection, sLight.direction); 
    // the factor will tell is wether or not we light the fragment in question
    // the rayDirection is where we are to the point illuminated, sLight.direction is the direction our light is facing in. 
    // if the angle between the 2 is small enough it means it should be illuminated (in the cone)

    if (slFactor > sLight.edge)
    {
        vec4 color = CalcPointLight(sLight.base, shadowIndex); // we pass in the pointLight part of our spotLight

        return  color*(1.0f - (1.0f- slFactor)*(1.0f/(1.0f - sLight.edge)));

        // We don't have a realistic spotLight (very sharp edge) so we need to create a faded effect close to the border 
        // for this we ant the ratio between the 2 scales (slFactor (0 to 1) and sLight.edge)
        //  1.0f - 0.0f/1.0f - sLight.edge : this will get the ratio between the 2 scales : how much we need to multiply this 1.0f - sLight.edge to get to 1.0f - 0.0f
        // to get the point we want (slFactor) on our new scale we multiply by (1.0f- slFactor)
        // we take the max of our new scale which is 1.0 and we substract from that the difference of our new scale : 1.0f - (1.0f- slFactor)*(1.0f/(1.0f - sLight.edge))
        // illustration : we have 2 scales :0-1 and 0-2 and we want 0.3 => we have 0.7 difference of difference with biggest for the 1st scale => we scale the 0.7 
        // in the scd. scale so we get 1.4 and we take the difference with the biggest value of the range which is 2 => 2-1.4 = 0.6 which is the equivalent of 0.3 but for the 2scd scale
    } else {
        return vec4 (0,0,0,0); 
    }

}

vec4 CalcPointLights()
{
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i=0; i < pointLightCount; i++)
    {
        totalColor += CalcPointLight(pointLights[i], i); // compute each light then adding whathever the result is for the fragment we are currently on. 
    }

    return totalColor; 
}

vec4 CalcSpotLights()
{
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i=0; i < spotLightCount; i++)
    {
        totalColor += CalcSpotLight(spotLights[i], i + pointLightCount); // compute each light then adding whathever the result is for the fragment we are currently on. 
    }

    return totalColor; 
}

void main(){       
    vec4 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights();
    finalColor += CalcSpotLights(); 
    color = texture(theTexture, TexCoord)*finalColor*vec4(objectColor, 1.0);
    color = mix(vec4(skyColor, 1.0), color, visibility); // 0 visibility would correspond to same color as the sky. 
}