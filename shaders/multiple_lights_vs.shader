#version 300 es

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 tex_coords;
out vec4 vertex;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat3 mInverseTranspose;

#define N 4

void main()
{
    vFragPos = vec3(mModel * vec4(aPos, 1.0));
    vNormal = mat3(transpose(inverse(mModel))) * aNormal;
    tex_coords = aTexCoords;

    vertex = vec4(aPos, 1.0);

    gl_Position = mProjection * mView * vec4(vFragPos, 1.0);
}
