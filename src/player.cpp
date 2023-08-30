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
    Collisions(fDeltaTime);
}



void Player::Collisions(float fDeltaTime)
{
    glm::vec2 vPlanarPos(vPos.x, vPos.z);
    //std::cout << "Current pos: " << glm::to_string(vPos) << std::endl;

    glm::vec2 vPotentialPos = vPlanarPos + vVel * fDeltaTime;
//    std::cout << "Potential pos: " << glm::to_string(vPotentialPos) << std::endl;

    glm::ivec2 vCurCell = glm::floor(vPlanarPos);
    glm::ivec2 vTargetCell = vPotentialPos;
    glm::ivec2 vAreaTL = glm::max(glm::min(vCurCell, vTargetCell) - glm::ivec2(1, 1), glm::ivec2(0, 0));
    glm::ivec2 vAreaBR = glm::min(glm::max(vCurCell, vTargetCell) + glm::ivec2(1, 1), pWorld->vWorldSize);

    for (auto &tile : pWorld->vTiles)
    {
        if (tile.bSolid)
            tile.vCol = glm::vec3(0.5f, 0.2f, 0.2f);
        else
            tile.vCol = glm::vec3(0.2f, 0.2f, 0.5f);
    }

    glm::ivec2 vCell;
    for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
    {
        for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
        {
            if (vCell.x >= pWorld->vWorldSize.x || vCell.y >= pWorld->vWorldSize.y)
                break;

            if (pWorld->vTiles[vCell.x * pWorld->vWorldSize.y + vCell.y].bSolid)
            {
                pWorld->vTiles[vCell.x * pWorld->vWorldSize.y + vCell.y].vCol = glm::vec3(0.5f, 0.2f, 0.2f);
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
                    pWorld->vTiles[vCell.x * pWorld->vWorldSize.y + vCell.y].vCol = glm::vec3(0.5f, 0.5f, 0.0f);
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
            else
                pWorld->vTiles[vCell.x * pWorld->vWorldSize.y + vCell.y].vCol = glm::vec3(0.0f, 0.5f, 0.0f);
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
        vVel = glm::normalize(vVel) * 4.0f;
}
