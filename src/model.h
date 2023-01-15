#pragma once

#include <vector> //to store the mesh list
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "texture.h"


class Model{

public:
    
    Model(); 

    void LoadModel(const std::string& fileName);
    void RenderModel(); 
    void ClearModel(); 



    ~Model(); 

private:

    void LoadNode(aiNode *node, const aiScene *scene); // to go through the tree
    void LoadMesh(aiMesh *mesh, const aiScene *scene); // the mesh will have to reference the scene to find the correct texture
    void LoadMaterials(const aiScene *scene);//we can have textures specific to specular lighting or dffuse lighting

    std::vector<Mesh*> meshList; 
    std::vector<Texture*> textureList; 
    std::vector<unsigned int> meshToTex; 

    // It is possible that soem mmesh may use the same texture. Instead of creating the same texture over and over we are going to create 
    // one of each texture and have the meshes say which one it is => meshes 1 and 2 may point to textureList 1 so for values 1 and 2 in mesToTex will be 1

}; 