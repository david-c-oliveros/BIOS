#include "object.h"


Object::Object()
{
}


Object::Object(std::string _sPath, glm::vec3 _vPos, float _fRotAngle)
    : vPos(_vPos), fRotAngle(_fRotAngle)
{
    m_pModel = std::make_unique<Model>(_sPath);
    vNextPos = vPos;
}


void Object::Draw(Shader &shader)
{
    glm::mat4 mModel = glm::mat4(1.0f);
    mModel = glm::rotate(mModel, glm::radians(fRotAngle), glm::vec3(1.0f, 1.0f, 0.0f));
    mModel = glm::translate(mModel, vPos);

    glm::mat3 vInverseTranspose = glm::transpose(glm::inverse(mModel));

    shader.SetMat4("mModel", mModel);
    shader.SetMat3("vInverseTranspose", vInverseTranspose);
    m_pModel->Draw(shader);
}
