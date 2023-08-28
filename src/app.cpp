#include "app.h"



Camera cCamera(glm::vec3(0.0f, 10.0f, 10.0f));
float fLast_X = 0.0f;
float fLast_Y = 0.0f;

bool bFirstMouse = true;

std::array<bool, 4> aKeyStates;



App::App(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight)
    : nCanvasWidth(_nCanvasWidth), nCanvasHeight(_nCanvasHeight)
{
}



App::~App()
{
    glfwTerminate();
}



void App::Create()
{
    Renderer::Init_GLFW(pWindow, nCanvasWidth, nCanvasHeight);
    Renderer::Init_WebGL(nCanvasWidth, nCanvasHeight, glContext, attrs);
    GLFWConfig();

    cWorld = std::make_unique<World>(16, 16);
    cCube = std::make_unique<Object>("/res/tile.obj");

    aKeyStates = { false, false, false, false };

    LoadShaders();
}


void App::Update()
{
    ProcessInput();
    Render();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}



void App::Render()
{
    Renderer::Clear(glm::vec4(0.1f, 0.1f, 0.3f, 1.0f));

    glm::mat4 mView = cCamera.GetViewMatrix();
    glm::mat4 mProjection = glm::perspective(glm::radians(cCamera.fZoom), (float)nCanvasWidth / (float)nCanvasHeight,  0.1f, 1000.0f);

    cShader.Use();
    cShader.SetMat4("mView", mView);
    cShader.SetMat4("mProjection", mProjection);
    cShader.SetVec3("vViewPos", cCamera.vPos);

    cWorld->Draw(cShader);
//    cCube->fRotAngle = 50.0f * glfwGetTime();
//    cCube->vPos = glm::vec3(0.0f, 0.0f, -10.0f);
//    cCube->Draw(cShader);
}



void App::LoadShaders()
{
    cShader.Create("shaders/multiple_lights_vs.shader", "shaders/multiple_lights_fs.shader");
    cShader.Use();

    cShader.SetVec3("sMaterial.diffuse", glm::vec3(0.5f, 0.0f, 0.0f));
    cShader.SetVec3("sMaterial.specular", glm::vec3(0.5f, 0.0f, 0.0f));
    cShader.SetFloat("sMaterial.shininess", 32.0f);
    cShader.SetVec3("sDirLight.direction", glm::vec3(0.5f, -0.5f, -0.1f));
    cShader.SetVec3("sDirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
    cShader.SetVec3("sDirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    cShader.SetVec3("sDirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    cShader.SetBool("bHasTexture", true);
    cShader.SetVec3("vColor", glm::vec3(0.2f, 0.1f, 0.1f));
    cShader.SetVec3("vFogColor", vFogColor);
}



void App::GLFWConfig()
{
    glfwSetFramebufferSizeCallback(pWindow, Framebuffer_Size_Callback);

    emscripten_set_mousemove_callback("#canvas", 0, 1, MouseCallback);
    emscripten_set_keydown_callback("#canvas", 0, 1, KeydownCallback);
    emscripten_set_keyup_callback("#canvas", 0, 1, KeyupCallback);

    glEnable(GL_DEPTH_TEST);
}



void App::ProcessInput()
{
    if (aKeyStates[(size_t)WASD::W])
        cCamera.ProcessKeyboard(Camera_Movement::FORWARD, 0.1, false);
    if (aKeyStates[(size_t)WASD::S])
        cCamera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.1, false);
    if (aKeyStates[(size_t)WASD::A])
        cCamera.ProcessKeyboard(Camera_Movement::LEFT, 0.1, false);
    if (aKeyStates[(size_t)WASD::D])
        cCamera.ProcessKeyboard(Camera_Movement::RIGHT, 0.1, false);
}



void Framebuffer_Size_Callback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}



EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent *e, void* userData)
{
    if (eventType != EMSCRIPTEN_EVENT_MOUSEMOVE)
        return 0;

    cCamera.ProcessMouseMovement(e->movementX, -e->movementY);

//    float xpos = static_cast<float>(e->movementX);
//    float ypos = static_cast<float>(e->movementY);

//    float xpos = e->movementX;
//    float ypos = e->movementY;
//
//    if (bFirstMouse)
//    {
//        fLast_X = xpos;
//        fLast_Y = xpos;
//        bFirstMouse = false;
//    }
//
//    float xoffset = xpos - fLast_X;
//    float yoffset = fLast_Y - ypos;
//
//    fLast_X = xpos;
//    fLast_Y = ypos;
//
//    cCamera.ProcessMouseMovement(xoffset, yoffset);

    return 0;
}



EM_BOOL KeydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
{
    if (!strcmp(e->key, "Enter"))
        emscripten_request_pointerlock("#canvas", 1);

    if (!strcmp(e->key, "w"))
        aKeyStates[(size_t)WASD::W] = true;
    if (!strcmp(e->key, "s"))
        aKeyStates[(size_t)WASD::S] = true;
    if (!strcmp(e->key, "a"))
        aKeyStates[(size_t)WASD::A] = true;
    if (!strcmp(e->key, "d"))
        aKeyStates[(size_t)WASD::D] = true;

    return 0;
}



EM_BOOL KeyupCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
{
    if (!strcmp(e->key, "Enter"))
        emscripten_request_pointerlock("#canvas", 1);

    if (!strcmp(e->key, "w"))
        aKeyStates[(size_t)WASD::W] = false;
    if (!strcmp(e->key, "s"))
        aKeyStates[(size_t)WASD::S] = false;
    if (!strcmp(e->key, "a"))
        aKeyStates[(size_t)WASD::A] = false;
    if (!strcmp(e->key, "d"))
        aKeyStates[(size_t)WASD::D] = false;

    return 0;
}



EM_BOOL PointerlockChangeCallback(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData)
{
    std::cout << "pointerlock change callback" << std::endl;
    emscripten_request_pointerlock("#canvas", 1);

    return 0;
}
