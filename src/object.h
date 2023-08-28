#ifndef OBJECT_H
#define OBJECT_H

#include "model.h"


class Object
{
    public:
        Object();
        Object(std::string sPath, glm::vec3 _vPos = glm::vec3(0.0f), float _fRotAngle = 0.0f);

        void Create(std::unique_ptr<Model> _pModel, glm::vec3 pos, float _fRotAngle);
        void Draw(Shader &cShader);


    public:
        glm::vec3 vPos;
        glm::vec3 vCol;
        float fRotAngle;


    private:
        std::unique_ptr<Model> m_pModel;
};

#endif
