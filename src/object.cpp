#include "object.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int Object::objectCounter = 0;

Object::Object() {}

Object::Object(int _geometry, Texture* tex, Material* matos, glm::vec3 obj_pos, glm::vec3 obj_rot, glm::vec3 obj_scale, glm::vec3 Color,  bool Bumpmap){
    type = _geometry;
    texture = tex;
    material = matos;
    position = obj_pos;
    rotation = obj_rot;
    scale = obj_scale;
    color = Color;
    id = objectCounter;
    bumpmap = Bumpmap;
    objectCounter++;  

    LoadVertices();
    numVertices = vertices.size();
    MakeObject();
}

void Object::AssignPoNoTe(Vertex* vertex, std::string f){
    std::string p, t, n;

    p = f.substr(0, f.find("/"));
    f.erase(0, f.find("/") + 1);

    t = f.substr(0, f.find("/"));
    f.erase(0, f.find("/") + 1);

    n = f.substr(0, f.find("/"));

    vertex->Position = vt_positions.at(std::stof(p) - 1);
    vertex->Normal = vt_normals.at(std::stof(n) - 1);
    vertex->Texture = vt_textures.at(std::stof(t) - 1);
}

void Object::AssignTangent(Vertex *v1, Vertex *v2, Vertex *v3){
    glm::vec3 Edge1 = v2->Position - v1->Position;
    glm::vec3 Edge2 = v3->Position - v1->Position;

    float DeltaU1 = v2->Texture.x - v1->Texture.x;
    float DeltaV1 = v2->Texture.y - v1->Texture.y;
    float DeltaU2 = v3->Texture.x - v1->Texture.x;
    float DeltaV2 = v3->Texture.y - v1->Texture.y;

    float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

    glm::vec3 tangent;

    tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
    tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
    tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

    v1->Tangent = tangent;
    v2->Tangent = tangent;
    v3->Tangent = tangent;
}

void Object::LoadVertices(){
    // Read the file defined by the path argument 
    // open the .obj file into a text editor and see how the data are organized
    // you will see line starting by v, vt, vn and f --> what are these ?
    // Then parse it to extract the data you need
    // keep track of the number of vertices you need

    std::ifstream infile(getGeometryPath());
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
        else if (indice == "f") { // for face
            std::string f1, f2, f3;
            iss >> f1 >> f2 >> f3;

            Vertex v1, v2, v3;
            AssignPoNoTe(&v1, f1);
            AssignPoNoTe(&v2, f2);
            AssignPoNoTe(&v3, f3);

            if (bumpmap){
                AssignTangent(&v1, &v2, &v3);
            }

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);
        }
    }
    std::cout << "Load model with " << vertices.size() << " vertices" << std::endl;

    infile.close();

}

void Object::MakeObject(){ 


    //has_texture = shader_texture;
    //Create the VAO and VBO
    //Put your data into your VBO
    //Define VBO and VAO as active buffer and active vertex array
    //Use the VAO to specify how your data should be read by your shader (glVertexAttribPointer and co)
    //Sometimes your shader will not use texture or normal attribute
    //you can use the boolean defined above to not specify these attribute 
    //desactive the buffer and delete the datas when your done

    float* data = getData();
    int size = 8; // pos is a vec3, tex is a vec2, normal is a vec3 (sum = 8)
    if (bumpmap) size += 3; // tangent = vec3 (sum += 3)


    
    glGenVertexArrays(1, &VAO); // first arg: amount of arrays we want, scd arg: ids of the arrays
    // bind the VAO: 
    glBindVertexArray(VAO);

    // define a buffer object to go inside the VAO: 
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
    //1st arg: location of the attribute, 2sd: size of the value passed here it is 3 because we pass x, y, z. 
    // 3rd: type of the values passed, 4th : if we want to normalise the values, 5th: with Opengl we can have colors and position int 
    //eh same valeus if we do so we should indicarte the amount of values we should skip to consider one vertex whole (in the vertices variable)
    // Here we are not doing that, 6th: offset which tells where the data starts, you can tell it it starts from 1 (so we basically ignore the first elment of variable vertices)

    glEnableVertexAttribArray(0); // Corresponds to the location of attribut (int he layout) directly linked to first arg of tehe previous command.

    glVertexAttribPointer(1, 2, GL_FLOAT, false, size * sizeof(float), (void*)(3 * sizeof(float)));
    // Arg 1: eaquals to 1 so we are dealing with texture coord. 
    // last arg: offset for the first value. 
    glEnableVertexAttribArray(1); 

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    if (bumpmap){
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
        // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void*)(11 * sizeof(float)));
        // glEnableVertexAttribArray(4);
    }

    //desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER,0); // bind to nothing => UNBIND
    glBindVertexArray(0); // UNBIND VAO
    delete[] data;

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

float* Object::getData(){
    float* data;
    if (bumpmap){
        data = new float[11 * numVertices];
        for (int i = 0; i < numVertices; i++) {
            Vertex v = vertices.at(i);
            data[i * 11] = v.Position.x;
            data[i * 11 + 1] = v.Position.y;
            data[i * 11 + 2] = v.Position.z;

            data[i * 11 + 3] = v.Texture.x;
            data[i * 11 + 4] = v.Texture.y;

            data[i * 11 + 5] = v.Normal.x;
            data[i * 11 + 6] = v.Normal.y;
            data[i * 11 + 7] = v.Normal.z;

            data[i * 11 + 8] = v.Tangent.x;
            data[i * 11 + 9] = v.Tangent.y;
            data[i * 11 + 10] = v.Tangent.z;
        }
    }
    else{
        data = new float[8 * numVertices];
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
    }
    return data;
}

void Object::draw(){

    //bind your vertex arrays and call glDrawArrays
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);

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

const char* Object::getGeometryPath() {
    if (type == geometry::domino)
        return geometryPath::domino;
    else if (type == geometry::plane) {
        scale.y = general::floorThickness;
        return geometryPath::cube;
    }
    else if (type == geometry::cube)
        return geometryPath::cube;
    else if (type == geometry::sphere)
        return geometryPath::sphere;
    return geometryPath::cube;

}

void Object::print(){
	std::cout << "object id="<< id << " (" << round(position.x) << ", " << round(position.z) << ", " << round(position.z) << ")" << std::endl;
}
