#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "shader.h"
#include "object.h"



enum class TileType
{
    NORMAL,
    SOLID,
    PORTAL
};



struct TileInst
{
    glm::ivec3 vPos;
    glm::vec3 vCol;
    TileType eType;
    bool bDebug;
};



class World
{
    public:
        World();
        ~World();

        void Draw(Shader &cShader);
        bool LoadLevel(std::string sPath);


    public:
        glm::ivec2 vWorldSize;
        std::vector<TileInst> vLevelTiles;
        std::unique_ptr<Object> cTileObj;
        glm::vec3 vFogColor = glm::vec3(0.01f);


    private:
        bool ProcessLevel(std::vector<std::string> vvLevelStr);
};
