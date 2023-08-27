#include "mesh.h"



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
//    : vbo([this]() -> GLuint { GLuint vbo; glGenBuffers(1, &vbo); return vbo; }()),
//      ebo([this]() -> GLuint { GLuint ebo; glGenBuffers(1, &ebo); return ebo; }())
{
    this->vertices = vertices;
    this->indices  = indices;
    this->textures = textures;

    //SetupMesh();
}


void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.Use();
        shader.SetFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    /***************************/
    /*        Draw Mesh        */
    /***************************/
    Renderer::Draw(vbo, shader.ID, indices.size());
    //glBindVertexArray(vao);
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}



void Mesh::SetupMesh()
{
//    glGenVertexArrays(1, &vao);
    std::cout << "will gen buffers" << std::endl;
    glGenBuffers(1, &vbo);
    std::cout << "gen vbo" << std::endl;
//    glGenBuffers(1, &ebo);

//    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    /**********************************/
    /*        Vertex Positions        */
    /**********************************/
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    /********************************/
    /*        Vertex Normals        */
    /********************************/
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    /***************************************/
    /*        Vertex Texture Coords        */
    /***************************************/
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    /********************************/
    /*        Vertex Tangent        */
    /********************************/
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    /**********************************/
    /*        Vertex Bitangent        */
    /**********************************/
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    /*********************/
    /*        IDs        */
    /*********************/
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    /*************************/
    /*        Weights        */
    /*************************/
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
}
