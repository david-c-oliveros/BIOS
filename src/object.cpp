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
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vPos);
    shader.SetMat4("model", model);
    m_pModel->Draw(shader);
}
