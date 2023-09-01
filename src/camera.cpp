#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : vFront(glm::vec3(0.0f, 0.0f, -1.0f)), fMovementSpeed(WALK_SPEED), fSprintSpeed(SPRINT_SPEED),
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
    : vFront(glm::vec3(0.0f, 0.0f, -1.0f)), fMovementSpeed(WALK_SPEED), fSprintSpeed(SPRINT_SPEED),
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
    //return glm::lookAt(vPos, vPos + vFront, vUp);
    return mView;
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, bool bDebug)
{
    float totalSpeed = bSprint ? fSprintSpeed * deltaTime : fMovementSpeed * deltaTime;
    if (bDebug)
        totalSpeed *= 4;

    if (direction == FORWARD)
        vPos += vFront * totalSpeed;
    if (direction == BACKWARD)
        vPos += -vFront * totalSpeed;
    if (direction == LEFT)
        vPos += -vRight * totalSpeed;
    if (direction == RIGHT)
        vPos += vRight * totalSpeed;

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



void Camera::OrbitFollow(std::shared_ptr<Object> vTarget, float fDeltaTime)
{
    fYaw = vTarget->fRotAngle;

    float dist = glm::distance(vPos, vTarget->vPos);
    glm::mat4 R = glm::yawPitchRoll(glm::radians(fYaw), 0.0f, 0.0f);

    glm::vec3 T = glm::vec3(-7.0f, dist, 0);
    T = glm::vec3(R * glm::vec4(T, 0.0f));
    glm::vec3 vNewPos = vTarget->vPos + T;
    vPos.x = glm::mix(vPos.x, vNewPos.x, fDeltaTime);
    vPos.y = glm::mix(vPos.y, 8.0f, fDeltaTime);
    vPos.z = glm::mix(vPos.z, vNewPos.z, fDeltaTime);
    vFront = glm::normalize(vTarget->vPos - vPos);

    vUp = glm::vec3(R * glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));
    vRight = glm::cross(vFront, vUp);
    mView = glm::lookAt(vPos, vTarget->vPos, vUp);
}



void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
    front.y = sin(glm::radians(fPitch));
    front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
    vFront = glm::normalize(front);

    vRight = glm::normalize(glm::cross(vFront, WorldUp));
    vUp    = glm::normalize(glm::cross(vRight, vFront));
}



void Camera::PrintvPos()
{
    std::cout << "(" << vPos.x << ", " << vPos.y << ", " << vPos.z << ")" << std::endl;
}
