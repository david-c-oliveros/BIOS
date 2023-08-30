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
    {
        std::cout << "ERROR: Failed to process level" << std::endl;
        return false;
    }

    return true;
}



bool World::ProcessLevel(std::vector<std::string> vLevelStr)
{
    glm::ivec2 vKeyIndex;
    glm::ivec2 vPortalIndex;

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
                    t.nID = nCurUUID++;
                    break;
                }

                case '#':
                {
                    t.vCol = glm::vec3(0.5f, 0.2f, 0.2f);
                    t.eType = TileType::SOLID;
                    t.bDebug = false;
                    t.nID = nCurUUID++;
                    break;
                }

                case '.':
                {
                    vPortalIndex = glm::ivec2(x, y);
                    t.vCol = glm::vec3(0.8f, 0.8f, 0.8f);
                    t.eType = TileType::PORTAL;
                    t.nID = nCurUUID++;
                    t.bDebug = false;
                    break;
                }

                case 'p':
                {
                    vKeyIndex = glm::ivec2(x, y);
                    t.vCol = glm::vec3(0.0f, 0.5f, 0.5f);
                    t.eType = TileType::PORTAL_KEY;
                    t.bDebug = false;
                    t.nID = nCurUUID++;
                    break;
                }

                default:
                {
                    std::cout << "Incorrect tile code: \"" << vLevelStr[x][y] << "\"" << std::endl;
                    std::cout << "At position: (" << x << ", " << y << ")" << std::endl;
                    return false;
                    break;
                }
            }

            vLevelTiles.push_back(t);
        }
    }

    int32_t nKeyID = vLevelTiles[vPortalIndex.y * vWorldSize.x + vPortalIndex.x].nID;
    if (nKeyID >= 0)
        vLevelTiles[vKeyIndex.y * vWorldSize.x + vKeyIndex.x].nPortalKey = nKeyID;

    return true;
}
