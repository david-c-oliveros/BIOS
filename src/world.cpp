#include "world.h"



World::World(uint32_t nWorldWidth, uint32_t nWorldLength)
{
    cTileObj = std::make_unique<Object>("/res/tile.obj");
    srand(123);
    for (int i = 0; i < nWorldWidth; i++)
    {
        for (int j = 0; j < nWorldLength; j++)
        {
            TileInst t;
            t.vPos = glm::vec3(i, 0, j);
            t.vCol = glm::vec3(glm::linearRand(0.0f, 1.0f),
                                 glm::linearRand(0.0f, 1.0f),
                                 glm::linearRand(0.0f, 1.0f));
            vTiles.push_back(t);
        }
    }
    
    std::cout << "World generated" << std::endl;
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
        cTileObj->Draw(cShader);
    }
}
