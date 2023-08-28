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
};



class World
{
    public:
        World(uint32_t nWorldWidth, uint32_t nWorldLength);
        ~World();

        void Draw(Shader &cShader);


    public:
        std::vector<TileInst> vTiles;
        std::unique_ptr<Object> cTileObj;
        glm::vec3 vFogColor = glm::vec3(0.01f);
};
