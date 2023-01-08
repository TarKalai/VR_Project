#version 330 core

in vec4 vertexColor;
in vec2 TexCoord; // will be interpollated if the points are not exactly on the expected points. //v_tex
in vec3 Normal; 
in vec3 FragPos;

out vec4 color;


const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 3;
const int MAX_AREA_LIGHTS = 100;

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

struct AreaLight{
    Light base;
    vec3 points[4];
    bool twoSided;
};

struct Material{
    float specularIntensity; 
    float shininess; 
    sampler2D diffuse;
    vec4 albedoRoughness; // (x,y,z) = color, w = roughness
};

uniform int pointLightCount;
uniform int spotLightCount; 
uniform int areaLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];  
uniform AreaLight areaLights[MAX_AREA_LIGHTS];

uniform sampler2D theTexture; 
uniform sampler2D LTC1; // for inverse M
uniform sampler2D LTC2; // GGX norm, fresnel, 0(unused), sphere
uniform Material material;
uniform vec3 areaLightTranslate;
uniform vec3 objectColor;

uniform vec3 eyePosition; // camera position (view position)

const float LUT_SIZE  = 64.0; // ltc_texture size
const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
const float LUT_BIAS  = 0.5/LUT_SIZE;

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

    return (ambientColor+diffuseColor)*(1+specularColor);

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
    // gamma correction
    if (areaLightCount>0){
        vec3 mDiffuse = texture(material.diffuse, TexCoord).xyz;// * vec3(0.7f, 0.8f, 0.96f);
        vec3 mSpecular = ToLinear(vec3(0.23, 0.23, 0.23)); // mDiffuse

        vec3 result = vec3(0.0);

        vec3 N = normalize(Normal);
        vec3 V = normalize(eyePosition - FragPos);
        vec3 P = FragPos;
        float dotNV = clamp(dot(N, V), 0.0, 1.0);

        // use roughness and sqrt(1-cos_theta) to sample M_texture
        vec2 uv = vec2(material.albedoRoughness.w, sqrt(1.0 - dotNV));
        uv = uv*LUT_SCALE + LUT_BIAS;

        // get 4 parameters for inverse_M
        vec4 t1 = texture(LTC1, uv);

        // Get 2 parameters for Fresnel calculation
        vec4 t2 = texture(LTC2, uv);

        mat3 Minv = mat3(
            vec3(t1.x, 0.0, t1.y),
            vec3(  0.0,  1.0,    0.0),
            vec3(t1.z, 0.0, t1.w)
        );

        // // translate light source for testing
        // vec3 translatedPoints[4];
        // translatedPoints[0] = areaLight.points[0] + areaLightTranslate;
        // translatedPoints[1] = areaLight.points[1] + areaLightTranslate;
        // translatedPoints[2] = areaLight.points[2] + areaLightTranslate;
        // translatedPoints[3] = areaLight.points[3] + areaLightTranslate;
        for (int i = 0; i < areaLightCount; i++){
            // Evaluate LTC shading

            vec3 diffuse = LTC_Evaluate(N, V, P, mat3(1), areaLights[i].points, areaLights[i].twoSided);
		    vec3 specular = LTC_Evaluate(N, V, P, Minv, areaLights[i].points, areaLights[i].twoSided);

            // GGX BRDF shadowing and Fresnel
            // t2.x: shadowedF90 (F90 normally it should be 1.0)
            // t2.y: Smith function for Geometric Attenuation Term, it is dot(V or L, H).
            specular *= mSpecular*t2.x + (1.0f - mSpecular) * t2.y;

            result += areaLights[i].base.color * areaLights[i].base.diffuseIntensity * (specular + mDiffuse * diffuse); // (specular + mDiffuse * diffuse)
        }
        return vec4(ToSRGB(result), 1.0f);
    }else {
        return vec4(0.0);
    }
}

void main(){

    vec4 finalColor = CalcDirectionalLight();
    
    finalColor += CalcPointLights();
    finalColor += CalcSpotLights(); 

    finalColor += CalcAreaLights();

    color = texture(theTexture, TexCoord)*finalColor*vec4(objectColor, 1.0);
}