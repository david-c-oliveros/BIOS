#include "world.h"



World::World()
{
    cTileObj = std::make_unique<Object>("/res/tile.obj");
    cWallObj = std::make_unique<Object>("/res/wall.obj");
}



World::~World()
{
}



void World::Draw(Shader &cShader)
{
    for (auto &tile : vLevelTiles)
    {
        if (tile.bDebug)
            cTileObj->vCol = glm::vec3(0.0f, 0.5f, 0.0f);

        switch(tile.eType)
        {
            case TileType::NORMAL:
            {
                cTileObj->vPos = glm::vec3(tile.vPos.x + 0.5f, tile.vPos.y, tile.vPos.z + 0.5f);
                cTileObj->vCol = tile.vCol;
                cTileObj->fShininess = 8.0f;
                cTileObj->Draw(cShader);
                break;
            }

            case TileType::SOLID:
            {
                cWallObj->vPos = glm::vec3(tile.vPos.x + 0.5f, tile.vPos.y, tile.vPos.z + 0.5f);
                cWallObj->vCol = tile.vCol;
                cWallObj->fShininess = 32.0f;
                cWallObj->Draw(cShader);
                break;
            }

            default:
            {
                cTileObj->vPos = glm::vec3(tile.vPos.x + 0.5f, tile.vPos.y, tile.vPos.z + 0.5f);
                cTileObj->vCol = tile.vCol;
                cTileObj->fShininess = 4.0f;
                cTileObj->Draw(cShader);
                break;
            }
        }
    }
}



void World::GenDebugWorld()
{
    vWorldSize = glm::ivec2(16, 16);
    for (int y = 0; y < vWorldSize.y; y++)
    {
        for (int x = 0; x < vWorldSize.x; x++)
        {
            TileInst t;
            t.vPos = glm::ivec3(x, 0, y);
            t.vCol = glm::ivec3(0.2f, 0.2f, 0.5f);
            t.bDebug = false;

            vLevelTiles.push_back(t);
        }
    }
}



bool World::LoadLevel(std::string sLevelPath, Shader &cShader)
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

    if (!ProcessLevel(vLevelStr, cShader))
    {
        std::cout << "ERROR: Failed to process level" << std::endl;
        return false;
    }

    return true;
}



bool World::ProcessLevel(std::vector<std::string> vLevelStr, Shader &cShader)
{
    glm::ivec2 vKeyIndex;
    glm::ivec2 vPortalIndex;

    for (int y = 0; y < vWorldSize.y; y++)
    {
        for (int x = 0; x < vWorldSize.x; x++)
        {
            TileInst t;
            t.vPos = glm::vec3(x, 0, y);

            switch(vLevelStr[y][x])
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

                case 'i':
                {
                    vPortalIndex = glm::ivec2(x, y);
                    t.vCol = glm::vec3(0.8f, 0.8f, 0.8f);
                    t.eType = TileType::SPAWN;
                    t.nID = nCurUUID++;
                    t.bDebug = false;
                    break;
                }

                case 'o':
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
                    cShader.SetVec3("sKeyLight.position", glm::vec3(t.vPos) + glm::vec3(0.5f, 0.0f, 0.5f));
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



void World::UnloadLevel()
{
    vLevelTiles.clear();
}



glm::ivec3 World::GetSpawnLoc()
{
    for (auto tile : vLevelTiles)
    {
        if (tile.eType == TileType::SPAWN)
            return tile.vPos;
    }

    return glm::ivec3(-1, 0, -1);
}
