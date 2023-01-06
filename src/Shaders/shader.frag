#version 330

in vec4 vertexColor;
in vec3 Normal; 
in vec3 FragPos; 
in vec4 DirectionalLightSpacePos; 

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

uniform sampler2D directionalShadowMap; 


// uniform sampler2D theTexture; 
uniform Material material; 

uniform vec3 eyePosition; // camera position



float CalcDirectionalShadowFactor(DirectionalLight light)
{

    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w; 
    // to get a new coordinate system and the coordiantes are between -1 and 1
    // need to convert to the scale 0-1 which is used by teh shadowMap
    projCoords = (projCoords * 0.5) + 0.5; 

   // float closest = texture(directionalShadowMap, projCoords.xy).r; THE PCF DEPTH IS THE CLOSEST VALUE NOW
    // get the closest depth = the closest position to the light at the moment (.r get the first value, stand for red)
    // so the lightsource know what is the closest point in the direction projCoords.xy by using the map directionalShadowMap BUT it is not necessarly the point we are looking at 
    // the point we are looking at is projCoords.xyz, so : 

    float current = projCoords.z; // how far away the point is from the light (forward and backward)

    // to correct the ACne problem we are going to add a bias:
    vec3 normal = normalize(Normal); // need the normal of the point we are checking
    vec3 lightDir= normalize(light.direction); 
    // we want to find the angle/factor between these 2 vectors which will be used to compute the bias: 
    // Normally bias = 0.05 is good enough, but computing it based on the angle is better as it adapts to where you are looking

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);

    // We are going to implement PCF (percentage Closer Filterring), so for a given pixel instead of taking it as it is, we are going to take all 8
    // pixels around it and we take the average for the shadow value=> this will allows us to have a better defined shadow (not pixelated) while 
    // using GL_NEAREST

    float shadow = 0.0; 

    // in order to move inside the texture we need the size of a single texel so we can move across by passing in the different x, y coord. 

    vec2 texelSize = 1.0/textureSize(directionalShadowMap, 0); 
    // textureSize is a built-in function
    // the last arg. is the mip map level we want to do the texelSize for. 
    // by doing this we will have 1 unit of the texel size

    // since we only need to move one pixel to the left and one pixel to the right, we are going to iterate from -1 to 1: 
    // we can increase the amount if we want more accuracy but it is expensive
    for(int x = -1; x <= 1; ++x )
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r; 
            // projCoords.xy + vec2(x,y): we add a bias which is our current x and y => tell us which direction the bias is in, then we need to know how far to go: 
            // "* texelSize" : to know how far to go. 
            // pcfDepth depth of the particular pixel

            shadow += current - bias > pcfDepth ? 1.0 : 0.0; 

        }
    }

    shadow /= 9.0; // divde by the number of pixels taken into account

    // float shadow = current - bias > closest ? 1.0 : 0.0;  dont't need this anymore: done in PCF
    // if it is the case, we want to say we are in full shadow: 1.0, otherwise : 0.0
    // if current = closest => means we have the same point, a point that can be seen by light => no shadow


    if(projCoords.z > 1.0)
    {
        shadow = 0.0; // when it is beyond the far plane then we say there is no shadow, we have to be sure ourselves that the farplane extends that far
    }

    return shadow; 

}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) // in the point light calculation there is a part where we need to compute the light based on the direction, directional light calculate all of it by direction. 
{
    vec4 ambientColor = vec4(light.color, 1.0f)*light.ambientIntensity; // convert the color into vec4

    float diffuseFactor = max(dot(normalize(Normal), -normalize(direction)), 0.0f); // 0 if it is neg. 
    // By taking the norm we have length of 1 for the magnitude of the vectors, so the result of 
    // dot product is: dot(A, B) = |A||B|cos(angle) where |A| = 1 = |B|, we effectivly get the angle. 

    vec4 diffuseColor = vec4(light.color, 1.0f)*light.diffuseIntensity * diffuseFactor; // diffuseFactor check if the light is at an angle that can be allowed to appear. 
    
    vec4 specularColor = vec4(0,0,0,0); 

    if(diffuseFactor>0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos); // we just want the direction where the fragment is from the eye
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal))); // we want ot know where the light ray is reflected around the normal. The first argument is what we want ot reflect and the scd is what we are reflecting it around. 
        // we are reflecting the light direction across the normal pointing out of the object. 
        // if fragToEye is the same as reflectedVertex then we will see bright light because specular is most intance at that point. 

        float specularFactor = dot(fragToEye, reflectedVertex); 

        if (specularFactor >0.0f){
            specularFactor = pow(specularFactor, material.shininess); //We update the specular factor
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);  
        }
    }

    return (ambientColor + (1.0 - shadowFactor) * (diffuseColor+specularColor));

}

vec4 CalcDirectionalLight()
{
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight); 

    return CalcLightByDirection(directionalLight.base, directionalLight.direction); 
} 

vec4 CalcPointLight(PointLight pLight)
// to calculate only one of the light so that if we are doing spotLight and it needs pointLight we can use this function
{
    vec3 direction = FragPos - pLight.position; // we get the vector from the pointLight to the fragment = direction
    float distance = length(direction); 
    direction = normalize(direction);

    vec4 color = CalcLightByDirection(pLight.base, direction); 
    float attenuation = pLight.exponent * distance * distance + pLight.linear*distance + pLight.constant;

    return (color/attenuation); // get the color of the pixel 

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

    color = finalColor;
}







//#version 330 core
//out vec4 FragColor;
//precision mediump float;
//in vec4 vertexColor; 
//void main() {
//FragColor = vec4(vertexColor.xyz, 1.0);
//FragColor = vertexColor; 
//}