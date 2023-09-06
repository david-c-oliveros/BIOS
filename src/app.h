#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/gl.h>

#include <array>
#include <memory>
#include <assert.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <webgl/webgl2.h>

#include <GLES3/gl3.h>

#include "miniaudio.h"

#include "renderer.h"
#include "shader.h"
#include "object.h"
#include "camera.h"
#include "world.h"
#include "player.h"



#define NUM_KEYS 64
#define CHANNELS 2
#define SAMPLE_RATE 48000



enum class KEYS
{
    W,
    A,
    S,
    D,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    P
};



enum class GameState
{
    MENU,
    CREDITS,
    RUNNING,
    WIN,
    DEBUG
};



void Framebuffer_Size_Callback(GLFWwindow* pWindow, int nWidth, int nHeight);
EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent *e, void* userData);
EM_BOOL KeydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
EM_BOOL KeyupCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
EM_BOOL PointerlockChangeCallback(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData);
void MA_DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount);
void MA_DataCallback_Decoder(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount);

bool MiniAudioLowLevel(const char* pPath);
bool MiniAudioHighLevel(const char* pPath);
float RandFloatInRange(float fMin, float fMax);



class App
{
    public:
        App(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~App();

        void Create();
        void GLFWConfig();
        void Update();
        void SetDeltaTime();
        void RenderMenu();
        void RenderGame();
        void RenderCreditsScreen();
        void RenderEndScreen();
        void CheckForSpecialTiles();
        bool CheckForIDMatch(TileInst &sTile);
        void PrintDebug();
        void LoadShaders();
        void LoadText();
        void ProcessInput();


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLuint vbo, vao, ebo;
        GLFWwindow* pWindow;

        GLTtext* pScreenText;
        glm::vec3 vTextColor = glm::vec3(1.0f);

    private:
        EmscriptenWebGLContextAttributes attrs;
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE glContext;

        glm::vec3 vFogColor = glm::vec3(0.1f, 0.1f, 0.3f);
};
