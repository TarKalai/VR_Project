#version 330 core

out vec4 color;

in vec2 TexCoord; 
in vec3 FragPos;
in mat3 TBN;
in vec4 DirectionalLightSpacePos; 
in float visibility; 

const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;
const int MAX_AREA_LIGHTS = 15;

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

struct AreaLight{
    Light base;
    vec3 points[4];
    bool twoSided;
};

struct Material{
    float specularIntensity; 
    float shininess;  
    float albedoRoughness; // (x,y,z) = color, w = roughness
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
uniform AreaLight areaLights[MAX_AREA_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform sampler2D directionalShadowMap; 
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS]; 

const float height_scale = 0.1;

uniform sampler2D LTC1;
uniform sampler2D LTC2;
uniform Material material; 

uniform vec3 eyePosition;
uniform vec3 objectColor;

const float LUT_SIZE  = 64.0;
const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
const float LUT_BIAS  = 0.5/LUT_SIZE;

vec3 sampleOffsetDirections[20] = vec3[]
(
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1), 
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), 
    vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1), 
    vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir){ 
    float height =  texture(depthMap, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
    return texCoords - p;    
} 

float CalcDirectionalShadowFactor(DirectionalLight light){
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w; 
    projCoords = (projCoords * 0.5) + 0.5; 
    float current = projCoords.z; 

    vec3 TangentLightPos = TBN * (-light.direction);
    vec3 TangentViewPos  = TBN * eyePosition;
    vec3 TangentFragPos  = TBN * FragPos;  
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec2 texCoords = ParallaxMapping(TexCoord,  lightDir);
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // vec3 lightDir= normalize(light.direction); 
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

    vec3 eyeDirection = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = ParallaxMapping(TexCoord,  eyeDirection);

    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  
    
    vec4 ambientColor = vec4(light.color, 1.0f)*light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(normal), normalize(eyeDirection)), 0.0); 
    vec4 diffuseColor = vec4(light.color, 1.0f)*light.diffuseIntensity * diffuseFactor;

    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    
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

vec4 saturate(vec4 totalColor) {
    totalColor.x = min(totalColor.x, 1);
    totalColor.y = min(totalColor.y, 1);
    totalColor.z = min(totalColor.z, 1);
    return totalColor;
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

// Vector form without project to the plane (dot with the normal)
// Use for proxy sphere clipping
vec3 IntegrateEdgeVec(vec3 v1, vec3 v2)
{
    // Using built-in acos() function will result flaws
    // Using fitting result for calculating acos()
    float x = dot(v1, v2);
    float y = abs(x);

    float a = 0.8543985 + (0.4965155 + 0.0145206*y)*y;
    float b = 3.4175940 + (4.1616724 + y)*y;
    float v = a / b;

    float theta_sintheta = (x > 0.0) ? v : 0.5*inversesqrt(max(1.0 - x*x, 1e-7)) - v;

    return cross(v1, v2)*theta_sintheta;
}

float IntegrateEdge(vec3 v1, vec3 v2)
{
    return IntegrateEdgeVec(v1, v2).z;
}

// P is fragPos in world space (LTC distribution)
vec3 LTC_Evaluate(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 points[4], bool twoSided)
{
    // construct orthonormal basis around N
    vec3 T1, T2;
    T1 = normalize(V - N * dot(V, N));
    T2 = cross(N, T1);

    // rotate area light in (T1, T2, N) basis
    Minv = Minv * transpose(mat3(T1, T2, N));

    // polygon (allocate 4 vertices for clipping)
    vec3 L[4];
    // transform polygon from LTC back to origin Do (cosine weighted)
    L[0] = Minv * (points[0] - P);
    L[1] = Minv * (points[1] - P);
    L[2] = Minv * (points[2] - P);
    L[3] = Minv * (points[3] - P);

    // use tabulated horizon-clipped sphere
    // check if the shading point is behind the light
    vec3 dir = points[0] - P; // LTC space
    vec3 lightNormal = cross(points[1] - points[0], points[3] - points[0]);
    bool behind = (dot(dir, lightNormal) < 0.0);

    // cos weighted space
    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);

    // integrate
    vec3 vsum = vec3(0.0);
    vsum += IntegrateEdgeVec(L[0], L[1]);
    vsum += IntegrateEdgeVec(L[1], L[2]);
    vsum += IntegrateEdgeVec(L[2], L[3]);
    vsum += IntegrateEdgeVec(L[3], L[0]);

    // form factor of the polygon in direction vsum
    float len = length(vsum);

    float z = vsum.z/len;
    if (behind)
        z = -z;

    vec2 uv = vec2(z*0.5f + 0.5f, len); // range [0, 1]
    uv = uv*LUT_SCALE + LUT_BIAS;

    // Fetch the form factor for horizon clipping
    float scale = texture(LTC2, uv).w;

    float sum = len*scale;
    if (!behind && !twoSided)
        sum = 0.0;

    // Outgoing radiance (solid angle) for the entire polygon
    vec3 Lo_i = vec3(sum, sum, sum);
    return Lo_i;
}

// PBR-maps for roughness (and metallic) are usually stored in non-linear
// color space (sRGB), so we use these functions to convert into linear RGB.
vec3 PowVec3(vec3 v, float p)
{
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float gamma = 1.0;
vec3 ToLinear(vec3 v) { return PowVec3(v, gamma); }
vec3 ToSRGB(vec3 v)   { return PowVec3(v, 1.0/gamma); }

vec4 CalcAreaLights(){
    if (areaLightCount>0){
        vec3 mSpecular = ToLinear(vec3(0.23, 0.23, 0.23)); 

        vec3 result = vec3(0.0);

        vec3 normal = texture(normalMap, TexCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0); 

        vec3 N = normalize(normal);
        vec3 V = normalize(eyePosition - FragPos);
        vec3 P = FragPos;
        float dotNV = clamp(dot(N, V), 0.0, 1.0);

        vec2 uv = vec2(material.albedoRoughness, sqrt(1.0 - dotNV));
        uv = uv*LUT_SCALE + LUT_BIAS;

        vec4 t1 = texture(LTC1, uv);
        vec4 t2 = texture(LTC2, uv);

        mat3 Minv = mat3(
            vec3(t1.x, 0.0, t1.y),
            vec3(  0.0,  1.0,    0.0),
            vec3(t1.z, 0.0, t1.w)
        );

        for (int i = 0; i < areaLightCount; i++){


            vec3 diffuse = LTC_Evaluate(N, V, P, mat3(1), areaLights[i].points, areaLights[i].twoSided);
		    vec3 specular = LTC_Evaluate(N, V, P, Minv, areaLights[i].points, areaLights[i].twoSided);

            specular *= mSpecular*t2.x + (1.0f - mSpecular) * t2.y;

            result += areaLights[i].base.color * areaLights[i].base.diffuseIntensity * (specular + diffuse); // (specular + mDiffuse * diffuse)
        }

        N = normalize(-normal);
        dotNV = clamp(dot(N, V), 0.0, 1.0);

        uv = vec2(material.albedoRoughness, sqrt(1.0 - dotNV));
        uv = uv*LUT_SCALE + LUT_BIAS;

        t1 = texture(LTC1, uv);
        t2 = texture(LTC2, uv);

        Minv = mat3(
            vec3(t1.x, 0.0, t1.y),
            vec3(  0.0,  1.0,    0.0),
            vec3(t1.z, 0.0, t1.w)
        );

        for (int i = 0; i < areaLightCount; i++){

            vec3 diffuse = LTC_Evaluate(N, V, P, mat3(1), areaLights[i].points, areaLights[i].twoSided);
		    vec3 specular = LTC_Evaluate(N, V, P, Minv, areaLights[i].points, areaLights[i].twoSided);
            specular *= mSpecular*t2.x + (1.0f - mSpecular) * t2.y;

            result += areaLights[i].base.color * areaLights[i].base.diffuseIntensity * (specular + diffuse); // (specular + mDiffuse * diffuse)
        }

        return vec4(ToSRGB(result), 1.0f);
    }else {
        return vec4(0.0);
    }
}

vec4 saturate2(vec4 totalColor){
    float maximum = max(max(totalColor.x, totalColor.y), totalColor.z);
    if (maximum>1){
        totalColor.x /= maximum;
        totalColor.y /= maximum;
        totalColor.z /= maximum;
        return totalColor;
    }
    return totalColor;
}

void main(){       
    vec4 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights();
    finalColor += saturate2(CalcSpotLights()); 

    finalColor += CalcAreaLights();

    vec3 TangentLightPos = TBN * (-directionalLight.direction*1000);
    vec3 TangentViewPos  = TBN * eyePosition;
    vec3 TangentFragPos  = TBN * FragPos;    
    vec3 lightDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = ParallaxMapping(TexCoord,  lightDir);


    color = texture(theTexture, texCoords)*finalColor*vec4(objectColor, 1.0);
    color = mix(vec4(skyColor, 1.0), color, visibility); // 0 visibility would correspond to same color as the sky. 
}