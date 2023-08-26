#pragma once

#include <iostream>

#include <GL/gl.h>

#include <memory>
#include <assert.h>


#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <webgl/webgl2.h>

//#include <GLES2/gl2.h>

#include "renderer.h"



void Framebuffer_Size_Callback(GLFWwindow* pWindow, int nWidth, int nHeight);
void processInput(GLFWwindow* pWindow);

void load_geo(float* vertices, unsigned int* indices);
void compile_shaders();



class App
{
    public:
        App(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~App();

        void Create();
        void Update();
        void CompileShaders();
        void load_geo(float* vertices, unsigned int* indices);


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLuint vbo, vao, ebo;
        GLFWwindow* pWindow;

        unsigned int shader;
        const char *vertexShaderSource =
            "attribute vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char *fragmentShaderSource =
        //    "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
            "}\n\0";



        EmscriptenWebGLContextAttributes attrs;
        std::unique_ptr<Renderer> cRenderer;
};
