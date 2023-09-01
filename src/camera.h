#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "object.h"


enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW          = 0.0f;
const float PITCH        = -45.0f;
const float WALK_SPEED   = 4.0f;
const float SPRINT_SPEED = 9.0f;
const float SENSITIVITY  = 0.05f;
const float ZOOM         = 45.0f;


class Camera
{
    public:
        glm::vec3 vPos;
        glm::vec3 vNextPos;
        glm::vec3 vVel;
        glm::vec3 vFront;
        glm::vec3 vUp;
        glm::vec3 vRight;
        glm::vec3 WorldUp;
        glm::mat4 mView;

        float fYaw;
        float fPitch;
        float fFloor;
        float fMovementSpeed;
        float fSprintSpeed;
        float fMouseSensitivity;
        float fZoom;

        bool bSprint;
        bool bConstrainToFloor;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = YAW, float pitch = PITCH);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        glm::mat4 GetViewMatrix();
        void ProcessKeyboard(Camera_Movement direction, float deltaTime, bool bDebug);
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch = true);
        void ProcessMouseScroll(float yoffset);
        void OrbitFollow(std::shared_ptr<Object> vTarget, float fDeltaTime);

        void PrintvPos();

    private:
        void UpdateCameraVectors();
};

#endif
