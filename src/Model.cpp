#include "Model.h"


Model::Model(std::string modelPath, MyCubeViewUI* uiStuff)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);
    
     meshes = std::vector<Mesh>();
    for (int i = 0; i < scene->mNumMeshes; i++) {
        Mesh newMesh = Mesh(scene->mMeshes[i], "error.png", uiStuff);
        meshes.push_back(newMesh);
    }
}

Model::~Model() {
}

void Model::Draw()
{
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw();
    }
}
