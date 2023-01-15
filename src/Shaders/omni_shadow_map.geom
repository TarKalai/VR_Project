#version 330
// three points are going to be passed as the data of a triangle. 
// will pass the data to the fragment shader : so the order 
layout (triangles) in; 
layout (triangle_strip, max_vertices=18) out; 
// instead of redifing another 3 poitns for another triangle, 
//we can use the points from the last triangle to figure out the next triangle
// we expect there will be a maximum of 18 vertices coming out of the geom. shader. If there is more it will be cut off

uniform mat4 lightMatrices[6]; 
// we need to render every side of the cube => we will have 6 light matrices that render for each individual one and pass that to our cube map 
// lightMatrix will be the combination of the projection and the view of the light source 

out vec4 FragPos; 

void main()
{
    for(int face = 0; face < 6; face++)
    {
        gl_Layer = face; 
        // define which of the textures we are going to output to, with the for loop we are going through each one. 
        for (int i = 0; i < 3; i++ ) // going through each of the vertices of the triangle passef from the vertex shader. 
        {
            FragPos = gl_in[i].gl_Position; 
            // gl_in will take layout (triangles) in, so the gl_in will have 3 values representing each point of the triangle being passed in
            
            gl_Position = lightMatrices[face] * FragPos;
            EmitVertex(); // emit vertex at the position gl_position for the layer face. 
        }
        EndPrimitive(); // xe finished to drax the triangle_strip and now we can go to another one. 
    }
}