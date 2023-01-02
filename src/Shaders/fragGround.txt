#version 330 core

in vec4 vertexColor;
in vec2 TexCoord; // will be interpollated if the points are not exactly on the expected points. //v_tex
in vec3 Normal; 
in vec3 FragPos;

out vec4 color;


const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 3;

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

struct PointLight 
{
    Light base; 
    vec3 position; 
    float constant;
    float linear; 
    float exponent; 

};

struct SpotLight
{
    PointLight base; 
    vec3 direction; 
    float edge;
};

struct Material{
    float specularIntensity; 
    float shininess; 
};

uniform int pointLightCount;
uniform int spotLightCount; 

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];  

uniform sampler2D theTexture; 
uniform Material material;

uniform vec3 eyePosition; // camera position

vec4 CalcLightByDirection(Light light, vec3 direction) // in the point light calculation there is a part where we need to compute the light based on the direction, directional light calculate all of it by direction. 
{
    vec4 ambientColor = vec4(light.color, 1.0f)*light.ambientIntensity; // convert the color into vec4

    float diffuseFactor = max(dot(normalize(Normal), -normalize(direction)), 0.0f); // 0 if it is neg. 
    // By taking the norm we have length of 1 for the magnitude of the vectors, so the result of 
    // dot product is: dot(A, B) = |A||B|cos(angle) where |A| = 1 = |B|, we effectivly get the angle. 

    vec4 diffuseColor = vec4(light.color, 1.0f)*light.diffuseIntensity * diffuseFactor; // diffuseFactor check if the light is at an angle that can be allowed to appear. 
    
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0); 

    if(diffuseFactor>0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos); // we just want the direction where the fragment is from the eye
        vec3 reflectedVertex = reflect(direction, normalize(Normal)); // we want ot know where the light ray is reflected around the normal. The first argument is what we want ot reflect and the scd is what we are reflecting it around. 
        // we are reflecting the light direction across the normal pointing out of the object. 
        // if fragToEye is the same as reflectedVertex then we will see bright light because specular is most intance at that point. 

        float specularFactor = dot(fragToEye, reflectedVertex); 

        if (specularFactor >0.0f){
            specularFactor = pow(specularFactor, material.shininess); //We update the specular factor
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);  
        }
    }

    return (ambientColor+diffuseColor+specularColor);

}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(directionalLight.base, directionalLight.direction); 
} 

vec4 CalcPointLight(PointLight pLight)
{
    vec4 totalColor = vec4(0.0, 0.0, 0.0, 0.0);

    vec3 direction = FragPos - pLight.position; // we get the vector from the pointLight to the fragment = direction
    float distance = length(direction); 
    direction = normalize(direction);

    vec4 color = CalcLightByDirection(pLight.base, direction); 
    float attenuation = pLight.exponent * distance * distance + pLight.linear*distance + pLight.constant;

    return (color/attenuation); 
}

vec4 CalcSpotLight(SpotLight sLight)
{
    
    vec3 rayDirection = normalize(FragPos - sLight.base.position); 
    // we need to know what the vector is between where we are and the fragment we are doing the calculation for
    float slFactor = dot(rayDirection, sLight.direction); 
    // the factor will tell is wether or not we light the fragment in question
    // the rayDirection is where we are to the point illuminated, sLight.direction is the direction our light is facing in. 
    // if the angle between the 2 is small enough it means it should be illuminated (in the cone)

    if (slFactor > sLight.edge)
    {
        vec4 color = CalcPointLight(sLight.base); // we pass in the pointLight part of our spotLight

        return  color*(1.0f - (1.0f- slFactor)*(1.0f/(1.0f - sLight.edge)));

        // We don't have a realistic spotLight (very sharp edge) so we need to create a faded effect close to the border 
        // for this we ant the ratio between the 2 scales (slFactor (0 to 1) and sLight.edge)
        //  1.0f - 0.0f/1.0f - sLight.edge : this will get the ratio between the 2 scales : how much we need to multiply this 1.0f - sLight.edge to get to 1.0f - 0.0f
        // to get the point we want (slFactor) on our new scale we multiply by (1.0f- slFactor)
        // we take the max of our new scale which is 1.0 and we substract from that the difference of our new scale : 1.0f - (1.0f- slFactor)*(1.0f/(1.0f - sLight.edge))
        // illustration : we have 2 scales :0-1 and 0-2 and we want 0.3 => we have 0.7 difference of difference with biggest for the 1st scale => we scale the 0.7 
        // in the scd. scale so we get 1.4 and we take the difference with the biggest value of the range which is 2 => 2-1.4 = 0.6 which is the equivalent of 0.3 but for the 2scd scale
    } else {
        return vec4 (0.0, 0.0, 0.0, 0.0); 
    }

}

vec4 CalcPointLights()
{
    vec4 totalColor = vec4(0.0, 0.0, 0.0, 0.0);
    for(int i=0; i < pointLightCount; i++)
    {
        totalColor += CalcPointLight(pointLights[i]); // compute each light then adding whathever the result is for the fragment we are currently on. 
    }

    return totalColor; 
}

vec4 CalcSpotLights()
{
    vec4 totalColor = vec4(0.0, 0.0, 0.0, 0.0);
    for(int i=0; i < spotLightCount; i++)
    {
        totalColor += CalcSpotLight(spotLights[i]); // compute each light then adding whathever the result is for the fragment we are currently on. 
    }

    return totalColor; 
}

void main(){

    vec4 finalColor = CalcDirectionalLight();
    
    finalColor += CalcPointLights();
    finalColor += CalcSpotLights(); 

    color = texture(theTexture, TexCoord)*finalColor;
}