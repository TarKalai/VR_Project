#include "model.h"

Model::Model(){}

void Model::RenderModel(){
    // we want to go through every mesh  and draw on the screen
    // check if the material is assigned for the mesh

    for(size_t i = 0; i < meshList.size(); i++){
        
        unsigned int materialIndex = meshToTex[i];

        if(materialIndex < textureList.size() && textureList[materialIndex]) // first we check if for this number it is possible to fit inside the textureList 
        {
            textureList[materialIndex]->UseTexture(); // attach the texture
        }

        meshList[i]->RenderMesh(); //redner the texture

    }
}

void Model::LoadModel(const std::string& fileName){

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices); 
    //aiProcess_Triangulate: in case there are complex shapes (not triangles used to make it) then this arg will force assimp to use triangles
    // aiProcess_FlipUVs : flip all the texture u, v along the y axis
    // aiProcess_GenSmoothNormals : we are using the concept of interpollation, we want smooth normals to have everything rounded and some object we will already have the normals defined but sometimes not so this will also generate the smooth normals for us
    // aiProcess_JoinIdenticalVertices: if there are 2 vertices at the same point, it will treat them as one vertices. 

    if (!scene){
        printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
        return; 
    }

    LoadNode(scene->mRootNode, scene); // the first node of the tree is mRootNode

    LoadMaterials(scene); 
}

void Model::LoadNode(aiNode *node, const aiScene *scene){

    for(size_t i=0; i< node->mNumMeshes; i++) //mNumMeshes : number of meshes at that node
    {

        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene); //node->mMeshes[i]] : holds the id of that mesh on that node, but the actual mesh is stored in scene so scene->mMeshes[node->mMeshes[i]], scene), the id is in the scene 

    }

    for(size_t i =0; i<node->mNumChildren; i++) //node->mNumChildren: number of childNodes. This is the recursion part
    {

        LoadNode(node->mChildren[i], scene);

    }
}

void Model::LoadMesh(aiMesh *mesh, const aiScene *scene)
// The way we store the mesh: we store the vertices as an array. Here we are going to create a vector and convert it to an array and pass that
// we do the same for indices
{

    std::vector<GLfloat> vertices; 
    std::vector<unsigned int> indices; 

    for(size_t i = 0;  i < mesh->mNumVertices; i++){

        vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}); 
        //insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}: we insert  at the end of the vector an array (x, y, z)
        if (mesh->mTextureCoords[0]) // we check if the first texture coord exist => does the mesh have a texture
        {
            vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y}); // 2D textures
        } 
        else{
            vertices.insert(vertices.end(), {0.0f, 0.0f});
        }
        vertices.insert(vertices.end(), {-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z});
        // insert the normals in reverse because in the fragShader we did float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f) instead of float diffuseFactor = max(dot(normalize(Normal), -normalize(direction)), 0.0f)
    }

    for(size_t i = 0; i < mesh->mNumFaces; i ++ ) // each face has 3 values for the indices that will indicate the appropriate mesh

    {
        aiFace face = mesh->mFaces[i]; 
        for(size_t j=0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* newMesh = new Mesh(); 
    newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size()); //vertices[0] : too get the location of the first vertices, the others are 1 after it
    meshList.push_back(newMesh);
    // we have to say which texture the mesh is pointing to
    meshToTex.push_back(mesh->mMaterialIndex); // mMaterialIndex: index of the material inthe material list
    //, when we take our materials and put those in the texture list we are going to make sure that each 
    //texture is at the same index as the one in ai.scene that way the materialIndex will pint to the same one 
    //that it pointed to when it was int he aiScene
}

void Model::LoadMaterials(const aiScene *scene)
// going through all the materials and we are ging to place them into the appropriate loocation in the textureList, we have to be sure that it has the same index as it has int he scene    
{

    textureList.resize(scene->mNumMaterials); // set a size beforehand for vectors => we create a bunch of textures in the texture list with an array size given in argument
    // tthen we can just do: tectureList[i] and set it to a value, that way, the textue we don't want to use will be set to Null

    for(size_t i =0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i]; // since we are accessing this from the scene we don't have to give it an id

        textureList[i] = nullptr; // null pointer

        if(material->GetTextureCount(aiTextureType_DIFFUSE)) // aiTextureType_EMISSIVE : will give off light ,  aiTextureType_OPACITY : for transparencey
        // aiTextureType_DIFFUSE : how the object will be colored if there is light
        {
            aiString path; 
            if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            // 1: type of texture we want, 2: which texture (we take the first here so 0), 3: where we are going to put the path (output)
            {
                int idx = std::string(path.data).rfind("\\"); // if we got the absolute path, then get rid off everything up until the last backslash
                // rfind : finds value starting from the end of the string, we put \\ because \ is an escape 
                std::string filename = std::string(path.data).substr(idx + 1);  // get a new string starting from just bewond the last /

                std::string texPath = std::string("../../src/Textures/") + filename; 

                textureList[i] = new Texture(texPath.c_str()); 

                if(!textureList[i]->LoadTexture()) // fail to load the texture
                {
                    printf("failed to load texture at: %s\n", texPath); 
                    delete textureList[i]; 
                    textureList[i] = nullptr; 
                }
            }
        }

        if (!textureList[i])
        {
            textureList[i] = new Texture("../../src/Textures/plain.png");
            textureList[i]->LoadTextureA();  
        }
    }
}

void Model::ClearModel(){

    for(size_t i = 0; i < meshList.size(); i++){
        if(meshList[i]){
            delete meshList[i]; 
            meshList[i] = nullptr; 
        }
    }

    for(size_t i = 0; i < textureList.size(); i++){
        if(textureList[i]){
            delete textureList[i]; 
            textureList[i] = nullptr; 
        }
    }
}

Model::~Model(){}