#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "object.h"
#include "world.h"



enum class EntityMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



class Player : public Object
{
    public:
        Player(std::shared_ptr<World> _pWorld, std::string _sPath, glm::vec3 _vPos = glm::vec3(0.0f), float _fRotAngle = 0.0f);
        ~Player();

        void Update(float fDeltaTime);
        void Collisions(float fDeltaTime);
        void DrawDebug(Shader &cShader);
        void CheckForSpecialTiles();
        void ProcessMovement(EntityMovement dir, float fDeltaTime);
        void Spawn(glm::vec3 vNewPos);
        void Draw(Shader &cShader);
        void UpdateVectors();
        void Orbit(glm::vec3 vTarget);


    public:
        glm::vec2 vVel = glm::vec2(0.0f, 0.0f);
        glm::vec3 vFront = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 vRight = glm::vec3(0.0f, 0.0f, 1.0f);
        std::shared_ptr<World> pWorld;

        std::vector<uint32_t> vPortalKeys;


    private:
        float fHeightOffset = 0.4f;
        float fColliderRadius = 0.2f;
        float fRotSpeed = 128.0f;
        float fSpeedScalar = 16.0f;

        glm::vec2 vImpulse = glm::vec2(0.0f, 0.0f);
        float fFriction = 0.9f;


    private:
        bool CheckForIDMatch(TileInst &sTile);
        void AddImpulse(glm::vec2 vImp);
};
