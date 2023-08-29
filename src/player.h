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
        void ProcessMovement(EntityMovement dir, float fDeltaTime);


    public:
        glm::vec2 vVel = glm::vec2(0.0f, 0.0f);
        glm::vec2 vFront = glm::vec2(1.0f, 0.0f);
        glm::vec2 vRight = glm::vec2(0.0f, 1.0f);
        std::shared_ptr<World> pWorld;


    private:
        float fColliderRadius = 0.7f;
};
