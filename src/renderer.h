#pragma once

#include <iostream>
#include <stdint.h>
#include <assert.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgl.h>
#include <webgl/webgl2.h>

#include <gltext.h>


class Renderer
{
    public:
        Renderer(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~Renderer();

        static void Clear(glm::vec4 vClearColor);
        static void Draw(GLuint &vao, GLuint &vbo, GLuint shader, GLuint nNumVert);
        static void DrawText(GLTtext* pScreenText, const char* pStr, glm::ivec2 vPos, glm::vec3 vColor, float fScale, bool bAligned);

        static bool Init_GLFW(GLFWwindow* pWindow, uint32_t nWidth, uint32_t nHeight);
        static bool Init_WebGL(uint32_t nWidth, uint32_t nHeight,
                               EMSCRIPTEN_WEBGL_CONTEXT_HANDLE &glContext,
                               EmscriptenWebGLContextAttributes &attrs);
        static bool Init_GLText();


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;


    private:
};
