#pragma once

#include <iostream>
#include <stdint.h>
#include <assert.h>

#include <GLFW/glfw3.h>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <webgl/webgl2.h>

static EMSCRIPTEN_WEBGL_CONTEXT_HANDLE glContext;


class Renderer
{
    public:
        Renderer(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~Renderer();

        static void Draw(GLuint &vbo, GLuint shader);
        static bool Init_GLFW(GLFWwindow* pWindow, uint32_t nWidth, uint32_t nHeight,
                              GLFWframebuffersizefun Framebuffer_Size_Callback);
        static bool Init_WebGL(uint32_t nWidth, uint32_t nHeight,
                               EmscriptenWebGLContextAttributes &attrs);


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
};
