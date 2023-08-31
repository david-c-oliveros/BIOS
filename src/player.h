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


    public:
        float fSpeedScalar = 4.0f;
        glm::vec2 vVel = glm::vec2(0.0f, 0.0f);
        glm::vec2 vFront = glm::vec2(1.0f, 0.0f);
        glm::vec2 vRight = glm::vec2(0.0f, 1.0f);
        std::shared_ptr<World> pWorld;

        std::vector<uint32_t> vPortalKeys;


    private:
        float fHeightOffset = 0.4f;
        float fColliderRadius = 0.9f;


    private:
        bool CheckForIDMatch(TileInst &sTile);
};
