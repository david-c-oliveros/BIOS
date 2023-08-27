#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(WALK_SPEED), SprintSpeed(SPRINT_SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM), bSprint(false), bConstrainToFloor(true)
{
    cCollider = BoxCollider(position, glm::vec3(1.0f, 2.0f, 1.0f));
    vPos = position;
    vNextPos = vPos;
    vVel = glm::vec3(0.0f);
    Floor = position.y;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    //pCollider = std::make_unique<BoxCollider>(vPos, glm::vec3(1.0f));
    updateCameraVectors();
}


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(WALK_SPEED), SprintSpeed(SPRINT_SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM), bSprint(false), bConstrainToFloor(true),
      cCollider(BoxCollider(glm::vec3(posX, posY, posZ), glm::vec3(1.0f)))
{
    vPos = glm::vec3(posX, posY, posZ);
    vNextPos = vPos;
    vVel = glm::vec3(0.0f);
    Floor = posY;
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}


glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(vPos, vPos + Front, Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, bool bDebug)
{
    float totalSpeed = bSprint ? SprintSpeed * deltaTime : MovementSpeed * deltaTime;
    if (bDebug)
        totalSpeed *= 4;

    if (direction == FORWARD)
        UpdateNextPos(Front * totalSpeed);
    if (direction == BACKWARD)
        UpdateNextPos(-(Front * totalSpeed));
    if (direction == LEFT)
        UpdateNextPos(-(Right * totalSpeed));
    if (direction == RIGHT)
        UpdateNextPos(Right * totalSpeed);

    if (direction == UP)
        UpdateNextPos(WorldUp * totalSpeed);
    if (direction == DOWN)
        UpdateNextPos(-(WorldUp * totalSpeed));

    if (bConstrainToFloor && !bDebug)
        vNextPos.y = Floor;
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (bConstrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}


void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;

    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}


void Camera::SetPosVel(glm::vec3 velocityVec)
{
    vVel = velocityVec;
    vPos += vVel;
    cCollider.UpdatePos(glm::vec3(vPos.x, vPos.y / 2, vPos.z));
}


void Camera::UpdatePos(glm::vec3 velocityVec)
{
    vPos += velocityVec;
    cCollider.UpdatePos(glm::vec3(vPos.x, vPos.y / 2, vPos.z));
}


void Camera::UpdateNextPos(glm::vec3 velocityVec)
{
    vVel = velocityVec;
    vNextPos += vVel;
    cCollider.UpdatePos(glm::vec3(vNextPos.x, vNextPos.y / 2, vNextPos.z));
}


void Camera::PrintvPos()
{
    std::cout << "(" << vPos.x << ", " << vPos.y << ", " << vPos.z << ")" << std::endl;
}
