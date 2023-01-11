#version 330

layout (location = 0) in vec3 pos; // position of the point on the skybox

out vec3 TexCoords; // is going to be the same as the pos, becuase the skybox is at the origin so the position  of a point is going to be : 
// the direction from the origin to that point so we can just take TexCoords as the position.

uniform mat4 projection;
uniform mat4 view; 

void main()
{
    TexCoords = pos; 
    gl_Position = projection * view * vec4(pos, 1.0); // don't use the model because don't move.
    // we have to take into account that because the camra is moving, the view matrix will keep 
    // changing based on the transformation of the camera, so we will be moving outside of the sampler cube and we have to avoid that
}