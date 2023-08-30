#include "player.h"



Player::Player(std::shared_ptr<World> _pWorld, std::string _sPath, glm::vec3 _vPos, float _fRotAngle)
    : Object(_sPath, _vPos, _fRotAngle), pWorld(_pWorld)
{
}



Player::~Player()
{
}



void Player::Update(float fDeltaTime)
{
    vVel *= fSpeedScalar;
    Collisions(fDeltaTime);
    vVel = glm::vec2(0.0f, 0.0f);
}



void Player::Collisions(float fDeltaTime)
{
    glm::vec2 vPlanarPos(vPos.x, vPos.z);

    glm::vec2 vPotentialPos = vPlanarPos + vVel * fDeltaTime;

    glm::ivec2 vCurCell = glm::floor(vPlanarPos);
    glm::ivec2 vTargetCell = vPotentialPos;
    glm::ivec2 vAreaTL = glm::max(glm::min(vCurCell, vTargetCell) - glm::ivec2(1, 1), glm::ivec2(0, 0));
    glm::ivec2 vAreaBR = glm::min(glm::max(vCurCell, vTargetCell) + glm::ivec2(1, 1), pWorld->vWorldSize);

    for (auto &tile : pWorld->vLevelTiles)
    {
        if (tile.eType == TileType::SOLID)
            tile.vCol = glm::vec3(0.5f, 0.2f, 0.2f);
        else if (tile.eType == TileType::NORMAL)
            tile.vCol = glm::vec3(0.2f, 0.2f, 0.5f);
        else
            tile.vCol = glm::vec3(0.8f, 0.8f, 0.8f);
    }

    glm::ivec2 vCell;
    for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
    {
        for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
        {
            if (vCell.x >= pWorld->vWorldSize.x || vCell.y >= pWorld->vWorldSize.y)
                break;

            int nCurIndex = vCell.y * pWorld->vWorldSize.x + vCell.x;
            if (pWorld->vLevelTiles[nCurIndex].eType == TileType::SOLID)
            {
                pWorld->vLevelTiles[nCurIndex].vCol = glm::vec3(0.5f, 0.2f, 0.2f);
                glm::vec2 vNearestPoint;

                /************************************************************************************************************/
                /*                                                                                                          */
                /*        This is the original algorithm from Javidx9, which, in my implementation,                         */
                /*        caused current solid cell plus next three to be "solid":                                           */
                /*        vNearestPoint.x = std::max(float(vCell.x), std::min(vPotentialPos.x, float(vCell.x + 1)));        */
                /*        vNearestPoint.y = std::max(float(vCell.y), std::min(vPotentialPos.y, float(vCell.y + 1)));        */
                /*        Below is my updated version, which works, but I'm not sure why, which troubles me                 */
                /*                                                                                                          */
                /************************************************************************************************************/

                vNearestPoint.x = std::max(float(vCell.x), std::min(vPotentialPos.x, float(vCell.x)));
                vNearestPoint.y = std::max(float(vCell.y), std::min(vPotentialPos.y, float(vCell.y)));

                glm::vec2 vRayToNearest = vNearestPoint - vPotentialPos;
                float fOverlap = fColliderRadius - glm::length(vRayToNearest);

                if (std::isnan(fOverlap))
                {
                    fOverlap = 0;
                }

                if (fOverlap > 0)
                {
                    pWorld->vLevelTiles[nCurIndex].vCol = glm::vec3(0.5f, 0.5f, 0.0f);
                    glm::vec2 vNorm = glm::normalize(vRayToNearest);

                    if (std::isnan(vNorm.x))
                    {
                        vNorm.x = 0.0;
                    }
                    if (std::isnan(vNorm.y))
                    {
                        vNorm.y = 0.0;
                    }

                    vPotentialPos = vPotentialPos - vNorm * fOverlap;
                }
            }
            else if (pWorld->vLevelTiles[nCurIndex].eType == TileType::PORTAL_KEY)
            {
                pWorld->vLevelTiles[nCurIndex].eType = TileType::NORMAL;
                vPortalKeys.push_back(pWorld->vLevelTiles[nCurIndex].nPortalKey);
            }
            else if (pWorld->vLevelTiles[nCurIndex].eType == TileType::PORTAL)
            {
                // Check if we have a Portal Key with an id that matches
                // that of the Portal, and if so, load into new level
                if (CheckForIDMatch(pWorld->vLevelTiles[nCurIndex]))
                    std::cout << "Time to load new level!" << std::endl;
                else
                    std::cout << "Locked! You must find the pointer to the next memory block" << std::endl;
            }
            else
                pWorld->vLevelTiles[nCurIndex].vCol = glm::vec3(0.0f, 0.5f, 0.0f);
        }
    }

    vPos.x = vPotentialPos.x;
    vPos.z = vPotentialPos.y;
}



void Player::ProcessMovement(EntityMovement eDir, float fDeltaTime)
{
    if (eDir == EntityMovement::FORWARD)
        vVel += vFront;
    if (eDir == EntityMovement::BACKWARD)
        vVel -= vFront;
    if (eDir == EntityMovement::RIGHT)
        vVel += vRight;
    if (eDir == EntityMovement::LEFT)
        vVel -= vRight;

    if (glm::length(vVel) * glm::length(vVel) > 0)
        vVel = glm::normalize(vVel);
}



bool Player::CheckForIDMatch(TileInst &sTile)
{
    for (auto &nKeyID : vPortalKeys)
    {
        if (nKeyID == sTile.nID)
            return true;
    }

    return false;
}
