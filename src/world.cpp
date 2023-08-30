#include "world.h"



World::World()
{
}



World::~World()
{
}



void World::Draw(Shader &cShader)
{
    for (auto &tile : vLevelTiles)
    {
        cTileObj->vPos = tile.vPos;
        cTileObj->vCol = tile.vCol;
        if (tile.bDebug)
            cTileObj->vCol = glm::vec3(0.0f, 0.5f, 0.0f);
        cTileObj->Draw(cShader);
    }
}



bool World::LoadLevel(std::string sLevelPath)
{
    std::ifstream fin;
    if (std::filesystem::exists(sLevelPath))
    {
        fin.open(sLevelPath);
    }

    std::string sLine;
    std::vector<std::string> vLevelStr;

    int x = 0;
    int y = 0;

    while (fin >> sLine)
    {
        vLevelStr.push_back(sLine);
        y++;
    }

    fin.close();

    vWorldSize = glm::ivec2(vLevelStr[0].length(), y);

    std::cout << glm::to_string(vWorldSize) << std::endl;

    if (!ProcessLevel(vLevelStr))
        return false;

    return true;
}



bool World::ProcessLevel(std::vector<std::string> vLevelStr)
{
    cTileObj = std::make_unique<Object>("/res/tile.obj");
    for (int y = 0; y < vWorldSize.y; y++)
    {
        for (int x = 0; x < vWorldSize.x; x++)
        {
            TileInst t;
            t.vPos = glm::vec3(x, 0, y);

            switch(vLevelStr[x][y])
            {
                case '=':
                {
                    t.vCol = glm::vec3(0.2f, 0.2f, 0.5f);
                    t.eType = TileType::NORMAL;
                    t.bDebug = false;
                    break;
                }

                case '#':
                {
                    t.vCol = glm::vec3(0.5f, 0.2f, 0.2f);
                    t.eType = TileType::SOLID;
                    t.bDebug = false;
                    break;
                }

                case '.':
                {
                    t.vCol = glm::vec3(0.8f, 0.8f, 0.8f);
                    t.eType = TileType::PORTAL;
                    t.bDebug = false;
                    break;
                }

                default:
                {
                    return false;
                    break;
                }
            }

            vLevelTiles.push_back(t);
        }
    }

    return true;
}
