#include "object.h"
#include "glm/ext.hpp" 
#include "glm/gtx/string_cast.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int Object::objectCounter = 0;

Object::Object() {}

Object::Object(const char* geometryPath, const char* texturePath, glm::vec3 obj_pos, glm::vec3 obj_rot, glm::vec3 obj_scale, bool is_visible){//, glm::vec3 Color){
    texturepath = texturePath;
    visible = is_visible;
    position = obj_pos;
    rotation = obj_rot;
    scale = obj_scale;
    //color = Color;
    id = objectCounter;
    objectCounter++;  

    // Read the file defined by the path argument 
    // open the .obj file into a text editor and see how the data are organized
    // you will see line starting by v, vt, vn and f --> what are these ?
    // Then parse it to extract the data you need
    // keep track of the number of vertices you need

    std::ifstream infile(geometryPath);
    //TODO Error management
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line); // Like a "split"
        std::string indice;
        iss >> indice; // Go the next elem of the split (reach elem 1 by 1 separated by " ")
        //std::cout << "indice : " << indice << std::endl;
        if (indice == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vt_positions.push_back(glm::vec3(x, y, z));

        }
        else if (indice == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            vt_normals.push_back(glm::vec3(x, y, z));
        }
        else if (indice == "vt") {
            float u, v;
            iss >> u >> v;
            vt_textures.push_back(glm::vec2(u, v));
        }
        else if (indice == "f") {
            std::string f1, f2, f3;
            iss >> f1 >> f2 >> f3;

            std::string p, t, n;

            //for face 1
            Vertex v1;

            p = f1.substr(0, f1.find("/"));
            f1.erase(0, f1.find("/") + 1);

            t = f1.substr(0, f1.find("/"));
            f1.erase(0, f1.find("/") + 1);

            n = f1.substr(0, f1.find("/"));


            v1.Position = vt_positions.at(std::stof(p) - 1);
            v1.Normal = vt_normals.at(std::stof(n) - 1);
            v1.Texture = vt_textures.at(std::stof(t) - 1);
            vertices.push_back(v1);

            //for face 2
            Vertex v2;

            p = f2.substr(0, f2.find("/"));
            f2.erase(0, f2.find("/") + 1);

            t = f2.substr(0, f2.find("/"));
            f2.erase(0, f2.find("/") + 1);

            n = f2.substr(0, f2.find("/"));


            v2.Position = vt_positions.at(std::stof(p) - 1);
            v2.Normal = vt_normals.at(std::stof(n) - 1);
            v2.Texture = vt_textures.at(std::stof(t) - 1);
            vertices.push_back(v2);

            //for face 3
            Vertex v3;

            p = f3.substr(0, f3.find("/"));
            f3.erase(0, f3.find("/") + 1);

            t = f3.substr(0, f3.find("/"));
            f3.erase(0, f3.find("/") + 1);

            n = f3.substr(0, f3.find("/"));


            v3.Position = vt_positions.at(std::stof(p) - 1);
            v3.Normal = vt_normals.at(std::stof(n) - 1);
            v3.Texture = vt_textures.at(std::stof(t) - 1);
            vertices.push_back(v3);
        }
    }
    std::cout << "Load model with " << vertices.size() << " vertices" << std::endl;

    infile.close();

    numVertices = vertices.size();
}

void Object::MakeObject(GLuint shaderID, bool shader_texture, bool shader_normal){ 
    has_texture = shader_texture;
    //Create the VAO and VBO
    //Put your data into your VBO
    //Define VBO and VAO as active buffer and active vertex array
    //Use the VAO to specify how your data should be read by your shader (glVertexAttribPointer and co)
    //Sometimes your shader will not use texture or normal attribute
    //you can use the boolean defined above to not specify these attribute 
    //desactive the buffer and delete the datas when your done

    float* data = new float[8 * numVertices];
    for (int i = 0; i < numVertices; i++) {
        Vertex v = vertices.at(i);
        data[i * 8] = v.Position.x;
        data[i * 8 + 1] = v.Position.y;
        data[i * 8 + 2] = v.Position.z;

        data[i * 8 + 3] = v.Texture.x;
        data[i * 8 + 4] = v.Texture.y;

        data[i * 8 + 5] = v.Normal.x;
        data[i * 8 + 6] = v.Normal.y;
        data[i * 8 + 7] = v.Normal.z;
    }
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //define VBO and VAO as active buffer and active vertex array
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, data, GL_STATIC_DRAW);

    {
        auto att_pos = glGetAttribLocation(shaderID, "pos"); //position
        glEnableVertexAttribArray(att_pos);
        glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
    }
    
    if (shader_texture) {
        glGenTextures(1, &texture);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nrComponents;
        unsigned char *data = stbi_load(texturepath, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum internalFormat;
            GLenum dataFormat;
            if (nrComponents == 1)
            {
                internalFormat = dataFormat = GL_RED;
            }
            else if (nrComponents == 3)
            {
                // internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                dataFormat = GL_RGB;
            }
            else if (nrComponents == 4)
            {
                // internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                dataFormat = GL_RGBA;
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            //3. Define the parameters for the texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            std::cout << "Failed to Load texture" << std::endl;
            const char* reason = stbi_failure_reason();
            std::cout << reason << std::endl;
        }

	    stbi_image_free(data);
        auto att_tex = glGetAttribLocation(shaderID, "tex");
        glEnableVertexAttribArray(att_tex);
        glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        u_texture = glGetUniformLocation(shaderID, "theTexture");
    }

    if (shader_normal) {
        auto att_col = glGetAttribLocation(shaderID, "norm"); //  "normal"
        glEnableVertexAttribArray(att_col);
        glVertexAttribPointer(att_col, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(1);
    }
    //desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    delete[] data;

    // Example to modify the position, rotation and scale of an object(to keep in case ^^)
    //model = glm::translate(model, position);
    //model = glm::rotate(model, (float) 3.14/5, glm::vec3(0.0f, 0.0f, 1.f));
    //model = glm::scale(model, glm::vec3(1,3,2));
    //std::cout << glm::to_string(model) << std::endl;

    setPosRot(position, rotation);
}

void Object::setPosRot(glm::vec3 obj_pos, glm::vec3 obj_rot) {
    position = obj_pos;
    rotation = obj_rot;
    float x1 = (cos(rotation.y)*cos(rotation.z)) * scale.x; 
    float x2 = (sin(rotation.x)*sin(rotation.y)*cos(rotation.z) - cos(rotation.x)*sin(rotation.z)) * scale.y;
    float x3 = (cos(rotation.x)*sin(rotation.y)*cos(rotation.z) + sin(rotation.x)*sin(rotation.z)) * scale.z;
    float y1 = (cos(rotation.y)*sin(rotation.z)) * scale.x;
    float y2 = (sin(rotation.x)*sin(rotation.y)*sin(rotation.z) + cos(rotation.x)*cos(rotation.z)) * scale.y;
    float y3 = (cos(rotation.x)*sin(rotation.y)*sin(rotation.z) - sin(rotation.x)*cos(rotation.z)) * scale.z;
    float z1 = (-sin(rotation.y)) * scale.x;
    float z2 = (sin(rotation.x)*cos(rotation.y)) * scale.y;
    float z3 = (cos(rotation.x)*cos(rotation.y)) * scale.z;
    model = glm::mat4(x1,y1,z1,0,x2,y2,z2,0,x3,y3,z3,0,position.x,position.y,position.z,1);
}

void Object::draw(){

    //bind your vertex arrays and call glDrawArrays
    glBindVertexArray(this->VAO);
    if (has_texture){
        glUniform1i(u_texture, 0);
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
    }
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

}

std::vector<glm::vec3> Object::getVertexPosition(){
    return vt_positions;
}

glm::vec3 Object::getPosition(){
    return position;
}

glm::vec3 Object::getRotation(){
    return rotation;
}
void Object::print(){
	std::cout << "object id="<< id << " (" << round(position.x) << ", " << round(position.z) << ", " << round(position.z) << ")" << std::endl;
}
