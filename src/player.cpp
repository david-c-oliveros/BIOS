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
    vVel += vImpulse;
    Collisions(fDeltaTime);
    CheckForSpecialTiles();
    vVel = glm::vec2(0.0f, 0.0f);
    vImpulse *= fFriction;
}



void Player::Collisions(float fDeltaTime)
{

    /****************************************/
    /*                                      */
    /*        Algorithm from Javidx9        */
    /*                                      */
    /****************************************/
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
                glm::vec2 vNearestPoint;

                vNearestPoint.x = std::max(float(vCell.x), std::min(vPotentialPos.x, float(vCell.x + 1)));
                vNearestPoint.y = std::max(float(vCell.y), std::min(vPotentialPos.y, float(vCell.y + 1)));

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
            else
                pWorld->vLevelTiles[nCurIndex].vCol = glm::vec3(0.0f, 0.5f, 0.0f);
        }
    }

    pWorld->vLevelTiles[vCurCell.y * pWorld->vWorldSize.x + vCurCell.x].vCol = glm::vec3(0.5f, 0.0f, 0.5f);

    vPos.x = vPotentialPos.x;
    vPos.z = vPotentialPos.y;
}



void Player::CheckForSpecialTiles()
{
    glm::vec2 vPlanarPos = glm::vec2(vPos.x, vPos.z);
    glm::vec2 vCurTile = glm::floor(vPlanarPos);

    int idx = vCurTile.y * pWorld->vWorldSize.x + vCurTile.x;

    if (pWorld->vLevelTiles[idx].eType == TileType::PORTAL_KEY)
    {
        pWorld->vLevelTiles[idx].eType = TileType::NORMAL;
        vPortalKeys.push_back(pWorld->vLevelTiles[idx].nPortalKey);
    }
    else if (pWorld->vLevelTiles[idx].eType == TileType::PORTAL)
    {
        // Check if we have a Portal Key with an id that matches
        // that of the Portal, and if so, load into new level
        if (CheckForIDMatch(pWorld->vLevelTiles[idx]))
        {
            pWorld->UnloadLevel();
            pWorld->LoadLevel("/res/level_02.lvl");
            Spawn(pWorld->GetSpawnLoc());
        }
        else
        {
            std::cout << "Locked! You must find the pointer to the next memory block" << std::endl;
        }
        pWorld->vLevelTiles[idx].vCol = glm::vec3(0.0f, 0.5f, 0.5f);
    }
    else
    {
        pWorld->vLevelTiles[idx].vCol = glm::vec3(0.5f, 0.0f, 0.5f);
    }
    pWorld->vLevelTiles[idx].vCol = glm::vec3(0.5f, 0.0f, 0.5f);

    pWorld->vLevelTiles[0].vCol = glm::vec3(0.0f);
}



void Player::ProcessMovement(EntityMovement eDir, float fDeltaTime)
{
    if (eDir == EntityMovement::FORWARD)
        AddImpulse(glm::vec2(vFront.x, vFront.z) * fDeltaTime * fSpeedScalar);
    if (eDir == EntityMovement::BACKWARD)
        AddImpulse(glm::vec2(-vFront.x, -vFront.z) * fDeltaTime * fSpeedScalar);

    if (eDir == EntityMovement::RIGHT)
        fRotAngle -= fRotSpeed * fDeltaTime;
    if (eDir == EntityMovement::LEFT)
        fRotAngle += fRotSpeed * fDeltaTime;

    if (glm::length(vVel) * glm::length(vVel) > 0)
        vVel = glm::normalize(vVel);

    UpdateVectors();
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



void Player::Spawn(glm::vec3 vNewPos)
{
    vPos = vNewPos;
}



void Player::Draw(Shader &cShader)
{
    vPos.y = fHeightOffset;
    Object::Draw(cShader);
}



void Player::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(-fRotAngle));
    front.y = 0.0f;
    front.z = sin(glm::radians(-fRotAngle));
    vFront = glm::normalize(front);

    vRight = glm::normalize(glm::cross(vFront, glm::vec3(0.0f, 1.0f, 0.0f)));
}



void Player::AddImpulse(glm::vec2 vImp)
{
    vImpulse += vImp;
    vVel += vImpulse;
}
