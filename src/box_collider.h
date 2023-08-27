#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <memory>
#include <glm/glm.hpp>

#include "model.h"
#include "object.h"


class BoxCollider
{
    public:
        glm::vec3 vDim;
        glm::vec3 vOrigin;
        glm::vec3 vMin;
        glm::vec3 vMax;

    public:
        BoxCollider(glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(2.0f, 2.0f, 2.0f));
        ~BoxCollider();

        void UpdatePos(glm::vec3 newPos);
        glm::vec3 GetPos();
};

#endif
