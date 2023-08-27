#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/BaseImporter.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "mesh.h"


unsigned int TextureFromFile(const char* path, const std::string &directory, bool gamma = false);


class Model
{
    public:
        Model(std::string const &path);

        void Draw(Shader &shader);

    private:
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;
        std::string directory;

        void LoadModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
