#include "object.h"


Object::Object()
{
}


Object::Object(std::string _sPath, glm::vec3 _vPos, float _fRotAngle)
    : vPos(_vPos), fRotAngle(_fRotAngle)
{
    m_pModel = std::make_unique<Model>(_sPath);
    fShininess = 32.0f;
}


void Object::Draw(Shader &cShader)
{
    glm::mat4 mModel = glm::mat4(1.0f);
    mModel = glm::translate(mModel, vPos);
    mModel = glm::rotate(mModel, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    cShader.Use();
    cShader.SetMat4("mModel", mModel);
    cShader.SetVec3("sMaterial.diffuse", vCol);
    cShader.SetVec3("sMaterial.specular", vCol);
    cShader.SetFloat("sMaterial.shininess", fShininess);
    m_pModel->Draw(cShader);
}
