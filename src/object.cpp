#include "object.h"

Object::Object(const char* path, glm::vec3 p, float s, int id){

    this->position = p;
    this->scale = s;
    this->id = id;

    // Read the file defined by the path argument 
    // open the .obj file into a text editor and see how the data are organized
    // you will see line starting by v, vt, vn and f --> what are these ?
    // Then parse it to extract the data you need
    // keep track of the number of vertices you need

    std::ifstream infile(path);
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


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //define VBO and VAO as active buffer and active vertex array
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Object::MakeObject(GLuint shaderID, bool shader_texture, bool shader_normal){ 

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
        auto att_pos = glGetAttribLocation(shaderID, "position");
        glEnableVertexAttribArray(att_pos);
        glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
    }
    
    if (shader_texture) {
        auto att_tex = glGetAttribLocation(shaderID, "tex_coord");
        glEnableVertexAttribArray(att_tex);
        glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        
    }
    
    if (shader_normal) {
        auto att_col = glGetAttribLocation(shaderID, "normal");
        glEnableVertexAttribArray(att_col);
        glVertexAttribPointer(att_col, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    }
    //desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    delete[] data;


    model = glm::scale(model, glm::vec3(scale));
    model = glm::translate(model, position);
}

void Object::draw(){

    //bind your vertex arrays and call glDrawArrays
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

}

void Object::print(){
	std::cout << "object id="<< id << " (" << round(position.x) << round(position.z) << round(position.z) << ")" << std::endl;
}