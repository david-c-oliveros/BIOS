#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "shader.h"
#include "object.h"



struct TileInst
{
    glm::ivec3 vPos;
    glm::vec3 vCol;
    bool bSolid;
    bool bDebug;
};



class World
{
    public:
        World(glm::ivec2 _vWorldSize);
        ~World();

        void Draw(Shader &cShader);


    public:
        glm::ivec2 vWorldSize;
        std::vector<TileInst> vTiles;
        std::unique_ptr<Object> cTileObj;
        glm::vec3 vFogColor = glm::vec3(0.01f);
};
