#pragma once

#include <GL/gl.h>

#include <string>
#include <vector>
#include <memory>

#include <glm/gtx/string_cast.hpp>

#include "renderer.h"
#include "shader.h"


#define MAX_BONE_INFLUENCE 4


struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};


struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};



class Mesh
{
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void Draw(Shader &shader);


    public:
        GLuint vao;
        GLuint vbo, ebo;

        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;


    private:
        void SetupMesh();

        bool bDebug = true;
};
