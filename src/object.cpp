#include "object.h"


Object::Object()
{
}


Object::Object(std::string _sPath, glm::vec3 _vPos, float _fRotAngle)
    : vPos(_vPos), fRotAngle(_fRotAngle)
{
    m_pModel = std::make_unique<Model>(_sPath);
}


void Object::Draw(Shader &cShader)
{
    glm::mat4 mModel = glm::mat4(1.0f);
    mModel = glm::rotate(mModel, glm::radians(fRotAngle), glm::vec3(1.0f, 1.0f, 0.0f));
    mModel = glm::translate(mModel, vPos);

    //std::cout << "pos: " << glm::to_string(vPos) << std::endl;
    //std::cout << "col: " << glm::to_string(vCol) << std::endl;

    cShader.Use();
    cShader.SetMat4("mModel", mModel);
    cShader.SetVec3("sMaterial.diffuse", vCol);
    cShader.SetVec3("sMaterial.specular", vCol);
    cShader.SetFloat("sMaterial.shininess", 32.0f);
    m_pModel->Draw(cShader);
}
