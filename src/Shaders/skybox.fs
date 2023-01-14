#version 330

in vec3 TexCoords; 

out vec4 color; 

uniform samplerCube skybox; 
// uniform vec3 fogColor; 

// const float lowerLimit = 0.0; // center of the skybox
// const float upperLimit = 30.0; // slightly above the horizon 

void main(void)
{
    // vec4 finalColor  = texture(skybox, TexCoords); // the color is the texture of the skybox at the coordinates: TexCoords.
    color  = texture(skybox, TexCoords);

    // float factor  = (TexCoords.y - lowerLimit) / (upperLimit - lowerLimit); // represents the visibility : if the factor = 0 => fragment
    // below the lower limit and is fog Color, factor of 1 => fragment above upperlimit use skybox texture color
    // use y of the texture coord as the hight of the fragment because texture coord are the same as the fragment position. 

    // factor  = clamp(factor, 0.0, 1.0); 
    // color = mix(vec4(fogColor, 1.0), color, factor);
}