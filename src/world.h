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
    PORTAL,
    SPAWN,
    PORTAL_KEY
};



struct TileInst
{
    glm::ivec3 vPos;
    glm::vec3 vCol;
    TileType eType;
    bool bDebug;
    int32_t nPortalKey = -1;
    uint32_t nID;
};



class World
{
    public:
        World();
        ~World();

        void GenDebugWorld();
        void Draw(Shader &cShader);
        bool LoadNextLevel(Shader &cShader);
        bool LoadLevel(std::string sPath, Shader &cShader);
        void UnloadLevel();
        void PrintLevelInfo();
        glm::ivec3 GetSpawnLoc();


    public:
        glm::ivec2 vWorldSize;
        std::vector<TileInst> vLevelTiles;
        std::unique_ptr<Object> cTileObj;
        std::unique_ptr<Object> cWallObj;
        glm::vec3 vFogColor = glm::vec3(0.01f);


    private:
        uint32_t nCurUUID = 0;
        uint32_t nCurLevel = 0;


    private:
        bool ProcessLevel(std::vector<std::string> vvLevelStr, Shader &cShader);
};
