#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), fMovementSpeed(WALK_SPEED), fSprintSpeed(SPRINT_SPEED),
      fMouseSensitivity(SENSITIVITY), fZoom(ZOOM), bSprint(false), bConstrainToFloor(true)
{
    vPos = position;
    vNextPos = vPos;
    vVel = glm::vec3(0.0f);
    fFloor = position.y;
    WorldUp = up;
    fYaw = yaw;
    fPitch = pitch;
    UpdateCameraVectors();
}


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), fMovementSpeed(WALK_SPEED), fSprintSpeed(SPRINT_SPEED),
      fMouseSensitivity(SENSITIVITY), fZoom(ZOOM), bSprint(false), bConstrainToFloor(true)
{
    vPos = glm::vec3(posX, posY, posZ);
    vNextPos = vPos;
    vVel = glm::vec3(0.0f);
    fFloor = posY;
    WorldUp = glm::vec3(upX, upY, upZ);
    fYaw = yaw;
    fPitch = pitch;
    UpdateCameraVectors();
}


glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(vPos, vPos + Front, Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, bool bDebug)
{
    float totalSpeed = bSprint ? fSprintSpeed * deltaTime : fMovementSpeed * deltaTime;
    if (bDebug)
        totalSpeed *= 4;

    if (direction == FORWARD)
        vPos += Front * totalSpeed;
    if (direction == BACKWARD)
        vPos += -Front * totalSpeed;
    if (direction == LEFT)
        vPos += -Right * totalSpeed;
    if (direction == RIGHT)
        vPos += Right * totalSpeed;

    if (bConstrainToFloor && !bDebug)
        vNextPos.y = fFloor;
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch)
{
    xoffset *= fMouseSensitivity;
    yoffset *= fMouseSensitivity;

    fYaw   += xoffset;
    fPitch += yoffset;

    if (bConstrainPitch)
    {
        if (fPitch > 89.0f)
            fPitch = 89.0f;
        if (fPitch < -89.0f)
            fPitch = -89.0f;
    }

    UpdateCameraVectors();
}


void Camera::ProcessMouseScroll(float yoffset)
{
    fZoom -= (float)yoffset;

    if (fZoom < 1.0f)
        fZoom = 1.0f;
    if (fZoom > 45.0f)
        fZoom = 45.0f;
}


void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
    front.y = sin(glm::radians(fPitch));
    front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}



void Camera::UpdateFollow(glm::vec3 vFollowPos, float fDeltaTime)
{
    vPos.x = glm::mix(vPos.x, vFollowPos.x - 4.0f, fDeltaTime * 4.0f);
    vPos.z = glm::mix(vPos.z, vFollowPos.z, fDeltaTime * 4.0f);
}



void Camera::PrintvPos()
{
    std::cout << "(" << vPos.x << ", " << vPos.y << ", " << vPos.z << ")" << std::endl;
}
