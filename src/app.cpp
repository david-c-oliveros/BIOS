#include "app.h"



/**********************************/
/*        Global Variables        */
/**********************************/

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;

Camera cCamera(glm::vec3(0.0f, 10.0f, 0.0f));
float fLast_X = 0.0f;
float fLast_Y = 0.0f;

bool bFirstMouse = true;

std::array<bool, 8> aKeyStates;

/**********************************/



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

    pWorld = std::make_shared<World>();
    if (!pWorld->LoadLevel("/res/level_01.lvl"))
        std::cout << "ERROR: Failed to load level" << std::endl;

    pCube = std::make_unique<Object>("/res/cube.obj");
    pCube->vPos = glm::vec3(0.0f, 0.0f, 0.0f);

    pPlayer = std::make_unique<Player>(pWorld, "/res/robot.obj");
    pPlayer->vPos = glm::vec3(8.5f, 0.4f, 8.5f);
    pPlayer->vCol = glm::vec3(0.3f, 0.3f, 0.45f);
    pPlayer->fShininess = 128.0f;

    for (int i = 0; i < 8; i++)
    {
        aKeyStates[i] = false;
    }

    LoadShaders();
}



/************************/
/*        Update        */
/************************/
void App::Update()
{
    SetDeltaTime();
    ProcessInput();
    pPlayer->Update(fDeltaTime);
    cCamera.UpdateFollow(pPlayer->vPos, fDeltaTime);
    Render();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}



void App::SetDeltaTime()
{
    float fCurrentFrame = static_cast<float>(glfwGetTime());
    fDeltaTime = fCurrentFrame - fLastFrame;
    fLastFrame = fCurrentFrame;
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

    pWorld->Draw(cShader);
    pPlayer->Draw(cShader);
    for (int y = 0; y < pWorld->vWorldSize.y; y++)
    {
        for (int x = 0; x < pWorld->vWorldSize.x; x++)
        {
            pCube->vPos = glm::vec3(x, 0.0f, y);
            pCube->Draw(cShader);
        }
    }
}



void App::PrintDebug()
{
    std::cout << "Keys obtained: " << pPlayer->vPortalKeys.size() << std::endl;
    for (auto &key : pPlayer->vPortalKeys)
        std::cout << "Key ID: " << key << std::endl;

    for (auto &tile : pWorld->vLevelTiles)
    {
        if (tile.eType == TileType::PORTAL)
            std::cout << "Portal ID: " << tile.nID << std::endl;
    }

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
    if (aKeyStates[(size_t)KEYS::W])
        cCamera.ProcessKeyboard(Camera_Movement::FORWARD, fDeltaTime, false);
    if (aKeyStates[(size_t)KEYS::S])
        cCamera.ProcessKeyboard(Camera_Movement::BACKWARD, fDeltaTime, false);
    if (aKeyStates[(size_t)KEYS::A])
        cCamera.ProcessKeyboard(Camera_Movement::LEFT, fDeltaTime, false);
    if (aKeyStates[(size_t)KEYS::D])
        cCamera.ProcessKeyboard(Camera_Movement::RIGHT, fDeltaTime, false);

    if (aKeyStates[(size_t)KEYS::UP])
        pPlayer->ProcessMovement(EntityMovement::FORWARD, fDeltaTime);
    if (aKeyStates[(size_t)KEYS::DOWN])
        pPlayer->ProcessMovement(EntityMovement::BACKWARD, fDeltaTime);
    if (aKeyStates[(size_t)KEYS::LEFT])
        pPlayer->ProcessMovement(EntityMovement::LEFT, fDeltaTime);
    if (aKeyStates[(size_t)KEYS::RIGHT])
        pPlayer->ProcessMovement(EntityMovement::RIGHT, fDeltaTime);
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

    return 0;
}



EM_BOOL KeydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
{
    if (!strcmp(e->key, "w"))
        aKeyStates[(size_t)KEYS::W] = true;
    if (!strcmp(e->key, "s"))
        aKeyStates[(size_t)KEYS::S] = true;
    if (!strcmp(e->key, "a"))
        aKeyStates[(size_t)KEYS::A] = true;
    if (!strcmp(e->key, "d"))
        aKeyStates[(size_t)KEYS::D] = true;

    if (!strcmp(e->key, "ArrowUp"))
        aKeyStates[(size_t)KEYS::UP] = true;
    if (!strcmp(e->key, "ArrowDown"))
        aKeyStates[(size_t)KEYS::DOWN] = true;
    if (!strcmp(e->key, "ArrowLeft"))
        aKeyStates[(size_t)KEYS::LEFT] = true;
    if (!strcmp(e->key, "ArrowRight"))
        aKeyStates[(size_t)KEYS::RIGHT] = true;

    return 0;
}



EM_BOOL KeyupCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
{
    if (!strcmp(e->key, " "))
        emscripten_request_pointerlock("#canvas", 1);

    if (!strcmp(e->key, "w"))
        aKeyStates[(size_t)KEYS::W] = false;
    if (!strcmp(e->key, "s"))
        aKeyStates[(size_t)KEYS::S] = false;
    if (!strcmp(e->key, "a"))
        aKeyStates[(size_t)KEYS::A] = false;
    if (!strcmp(e->key, "d"))
        aKeyStates[(size_t)KEYS::D] = false;

    if (!strcmp(e->key, "ArrowUp"))
        aKeyStates[(size_t)KEYS::UP] = false;
    if (!strcmp(e->key, "ArrowDown"))
        aKeyStates[(size_t)KEYS::DOWN] = false;
    if (!strcmp(e->key, "ArrowLeft"))
        aKeyStates[(size_t)KEYS::LEFT] = false;
    if (!strcmp(e->key, "ArrowRight"))
        aKeyStates[(size_t)KEYS::RIGHT] = false;

    return 0;
}



EM_BOOL PointerlockChangeCallback(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData)
{
    emscripten_request_pointerlock("#canvas", 1);

    return 0;
}
