#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/gl.h>

#include <array>
#include <memory>
#include <assert.h>

#include <GLFW/glfw3.h>

#include <box2d/box2d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <webgl/webgl2.h>

#include <GLES3/gl3.h>

#include "renderer.h"
#include "shader.h"
#include "object.h"
#include "camera.h"
#include "world.h"
#include "player.h"



enum class KEYS
{
    W,
    A,
    S,
    D,
    UP,
    DOWN,
    LEFT,
    RIGHT
};


void Framebuffer_Size_Callback(GLFWwindow* pWindow, int nWidth, int nHeight);
EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent *e, void* userData);
EM_BOOL KeydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
EM_BOOL KeyupCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
EM_BOOL PointerlockChangeCallback(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData);



class App
{
    public:
        App(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~App();

        void Create();
        void GLFWConfig();
        void Update();
        void SetDeltaTime();
        void Render();
        void PrintDebug();
        void LoadShaders();
        void ProcessInput();
        void InitBox2D();


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLuint vbo, vao, ebo;
        GLFWwindow* pWindow;

        Shader cShader;

        std::shared_ptr<World> pWorld;
        std::unique_ptr<Player> pPlayer;

        GLuint shader;

    private:
        EmscriptenWebGLContextAttributes attrs;
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE glContext;

        glm::vec3 vFogColor = glm::vec3(0.1f, 0.1f, 0.3f);
};
