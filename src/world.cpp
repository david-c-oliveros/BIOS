#include "world.h"



World::World(glm::ivec2 _vWorldSize)
    : vWorldSize(_vWorldSize)
{
    cTileObj = std::make_unique<Object>("/res/tile.obj");
    srand(123);
    for (int i = 0; i < vWorldSize.x; i++)
    {
        for (int j = 0; j < vWorldSize.y; j++)
        {
            TileInst t;
            t.vPos = glm::vec3(i, 0, j);

            if (i % 3 == 0 && j % 3 == 0)
            {
                t.vCol = glm::vec3(0.5f, 0.2f, 0.2f);
                t.bSolid = true;
                t.bDebug = false;
            }
            else
            {
                t.vCol = glm::vec3(0.2f, 0.2f, 0.5f);
                t.bSolid = false;
                t.bDebug = false;
            }

            vTiles.push_back(t);
        }
    }
}



World::~World()
{
}



void World::Draw(Shader &cShader)
{
    for (auto &tile : vTiles)
    {
        cTileObj->vPos = tile.vPos;
        cTileObj->vCol = tile.vCol;
        if (tile.bDebug)
            cTileObj->vCol = glm::vec3(0.0f, 0.5f, 0.0f);
        cTileObj->Draw(cShader);
    }
}
