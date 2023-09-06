#define MINIAUDIO_IMPLEMENTATION
#define GLT_IMPLEMENTATION

#include "app.h"



/**********************************/
/*        Global Variables        */
/**********************************/
const char* pMusic = "/res/audio/dark-ambient.wav";

GameState eState;

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;

Camera cCamera(glm::vec3(-0.05f, 0.2f, 1.5f), glm::vec3(0.0f, 1.0f, 0.0f), 270.0f, 0.0f);

float fLast_X = 0.0f;
float fLast_Y = 0.0f;

bool bFirstMouse = true;


Shader cShader;
std::shared_ptr<World> pWorld;
std::shared_ptr<Player> pPlayer;
std::shared_ptr<Object> pMenuRobot;

std::array<bool, NUM_KEYS> aKeyStates;

/**********************************/



/******************************/
/*        Audio Engine        */
/******************************/
static ma_engine gEngine;
static ma_sound gSound;

static ma_context gContext;
static ma_decoder gDecoder;

static ma_device gDevice;



App::App(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight)
    : nCanvasWidth(_nCanvasWidth), nCanvasHeight(_nCanvasHeight)
{
}



App::~App()
{
    ma_device_uninit(&gDevice);
    ma_decoder_uninit(&gDecoder);
    ma_context_uninit(&gContext);
    glfwTerminate();
}



void App::Create()
{
    Renderer::Init_GLFW(pWindow, nCanvasWidth, nCanvasHeight);
    Renderer::Init_WebGL(nCanvasWidth, nCanvasHeight, glContext, attrs);
    Renderer::Init_GLText();
    GLFWConfig();

    LoadShaders();

    pScreenText = gltCreateText();

    eState = GameState::MENU;

    pMenuRobot = std::make_shared<Object>("/res/models/robot_menu.obj");
    pMenuRobot->vPos = glm::vec3(0.0f);
    pMenuRobot->vCol = glm::vec3(0.1f, 0.3f, 0.3f);
    pMenuRobot->fShininess = 0.0f;

    for (int i = 0; i < NUM_KEYS; i++)
    {
        aKeyStates[i] = false;
    }

}



/************************/
/*        Update        */
/************************/
void App::Update()
{
    SetDeltaTime();
    ProcessInput();

    switch(eState)
    {
        case GameState::MENU:
        {
//            RenderMenu();
            RenderEndScreen();
            break;
        }

        case GameState::CREDITS:
        {
            RenderCreditsScreen();
            break;
        }

        case GameState::RUNNING:
        {
            pPlayer->Update(fDeltaTime);
            CheckForSpecialTiles();
            cCamera.OrbitFollow(pPlayer, fDeltaTime);
            RenderGame();
            break;
        }

        case GameState::WIN:
        {
            RenderEndScreen();
        }
    }

    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}



void App::SetDeltaTime()
{
    float fCurrentFrame = static_cast<float>(glfwGetTime());
    fDeltaTime = fCurrentFrame - fLastFrame;
    fLastFrame = fCurrentFrame;
}



void App::RenderMenu()
{
    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.1f, 1.0f));

    glm::mat4 mView = cCamera.GetViewMatrix();
    glm::mat4 mProjection = glm::perspective(glm::radians(cCamera.fZoom), (float)nCanvasWidth / (float)nCanvasHeight,  0.1f, 1000.0f);

    cShader.Use();
    cShader.SetMat4("mView", mView);
    cShader.SetMat4("mProjection", mProjection);
    cShader.SetVec3("vViewPos", cCamera.vPos);
    cShader.SetVec3("sPlayerLight.position", pMenuRobot->vPos + glm::vec3(-0.5f, 0.0f, 0.0f));
    cShader.SetFloat("fPlayerLightScalar", 0.5f);

    pMenuRobot->Draw(cShader);

    glm::ivec2 vOffset(0, nCanvasWidth * 0.041667);     // 60
    glm::ivec2 vDescPos(nCanvasWidth / 8 + 80, nCanvasHeight / 12);
    glm::ivec2 vMenuPos(nCanvasWidth / 1.4, nCanvasHeight / 3);

    float fScale = nCanvasWidth * 0.005556; // 8.0
    Renderer::DrawText(pScreenText, "Bios", glm::ivec2(nCanvasWidth / 1.4, nCanvasHeight / 6), glm::vec3(1.0f, 1.0f, 1.0f), fScale, true);

    fScale = nCanvasWidth * 0.001736;       // 2.5
    Renderer::DrawText(pScreenText, "Bios has just",         vDescPos,               glm::vec3(1.0f, 1.0f, 1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "experienced a crash!",  vDescPos + vOffset,     glm::vec3(1.0f, 1.0f, 1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "Help him get back",     vDescPos + vOffset * 2, glm::vec3(1.0f, 1.0f, 1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "home to his partition", vDescPos + vOffset * 3, glm::vec3(1.0f, 1.0f, 1.0f), fScale, true);

    fScale = nCanvasWidth * 0.002778;       // 4.0
    vOffset.y = nCanvasWidth * 0.048611;    // 70
    Renderer::DrawText(pScreenText, "[Enter] - Start Game", vMenuPos,           glm::vec3(1.0f, 1.0f, 1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "   [C] - Credits",     vMenuPos + vOffset, glm::vec3(1.0f, 1.0f, 1.0f), fScale, true);

    fScale = nCanvasWidth * 0.002431;       // 3.5
    Renderer::DrawText(pScreenText, "Controls - WASD",      vMenuPos + vOffset *  3, glm::vec3(1.0f,  1.0f, 1.0f), fScale, true);

    fScale = nCanvasWidth * 0.001726;       // 2.5
    vOffset.y = nCanvasWidth * 0.034722;    // 50
    Renderer::DrawText(pScreenText, "Collect the pointers", vMenuPos + vOffset *  6, glm::vec3(1.0f,  1.0f, 1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "to access the next",   vMenuPos + vOffset *  7, glm::vec3(1.0f,  1.0f, 1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "block of memory",      vMenuPos + vOffset *  8, glm::vec3(1.0f,  1.0f, 1.0f), fScale, true);

    Renderer::DrawText(pScreenText, "Pointer tiles",        vMenuPos + vOffset * 10, glm::vec3(0.06f, 1.0f, 1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "Exit portal tiles",    vMenuPos + vOffset * 11, glm::vec3(0.06f, 1.0f, 0.0f), fScale, true);
}



void App::RenderCreditsScreen()
{
    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.1f, 1.0f));

    glm::ivec2 vOffset(0, nCanvasWidth * 0.055556);     // 80
    glm::ivec2 vTitlePos(nCanvasWidth / 2, nCanvasHeight / 6);

    float fScale = nCanvasWidth * 0.004167;             // 6.0
    Renderer::DrawText(pScreenText, "Credits",                              vTitlePos, glm::vec3(1.0f), fScale, true);

    fScale = nCanvasWidth * 0.001726;                   // 2.5
    Renderer::DrawText(pScreenText, "GLFW - glfw.org",                      vTitlePos + vOffset * 2, glm::vec3(1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "Emscripten - emscripten.org",          vTitlePos + vOffset * 3, glm::vec3(1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "GLM - github.com/g-truc/glm",          vTitlePos + vOffset * 4, glm::vec3(1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "Assimp - assimp.org",                  vTitlePos + vOffset * 5, glm::vec3(1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "glText - gitbub.com/vallentin/glText", vTitlePos + vOffset * 6, glm::vec3(1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "Collision algorithm from Javidx9",     vTitlePos + vOffset * 7, glm::vec3(1.0f), fScale, true);

    fScale = nCanvasWidth * 0.002431;                   // 3.5
    Renderer::DrawText(pScreenText, "[Enter] - Back",     vTitlePos + vOffset * 9, glm::vec3(1.0f), fScale, true);
}



void App::RenderEndScreen()
{
    glm::ivec2 vOffset(0, nCanvasWidth * 0.034722);     // 50
    glm::ivec2 vTitlePos(nCanvasWidth / 2, nCanvasHeight / 2);

    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.1f, 1.0f));

    float fScale = nCanvasWidth * 0.005556;
    Renderer::DrawText(pScreenText, "Success!", vTitlePos - vOffset * 2, glm::vec3(1.0f), fScale, true);

    fScale = nCanvasWidth * 0.002778;
    Renderer::DrawText(pScreenText, ("Your time: " + std::to_string(pPlayer->nRunTime)).c_str(), vTitlePos + vOffset,     glm::vec3(1.0f), fScale, true);
    Renderer::DrawText(pScreenText, "Press Enter to return to menu",                             vTitlePos + vOffset * 4, glm::vec3(1.0f), fScale, true);
}



void App::RenderGame()
{
    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.1f, 1.0f));

    glm::mat4 mView = cCamera.GetViewMatrix();
    glm::mat4 mProjection = glm::perspective(glm::radians(cCamera.fZoom), (float)nCanvasWidth / (float)nCanvasHeight,  0.1f, 1000.0f);

    cShader.Use();
    cShader.SetMat4("mView", mView);
    cShader.SetMat4("mProjection", mProjection);
    cShader.SetVec3("vViewPos", cCamera.vPos);
    cShader.SetVec3("sPlayerLight.position", pPlayer->vPos + glm::vec3(0.0f, 0.7f, 0.0f));

    if ((uint32_t)(glfwGetTime() * 10000) % 8 == 0)
    {
        srand((uint32_t)(glfwGetTime() * 10000));
        cShader.SetFloat("fPlayerLightScalar", RandFloatInRange(0.85f, 1.0f));
    }

    pWorld->Draw(cShader);
    pPlayer->Draw(cShader);

    Renderer::DrawText(pScreenText, "Time Elapsed: ",
                       glm::ivec2(nCanvasWidth / 32, nCanvasHeight / 32),
                       glm::vec3(1.0f, 1.0f, 1.0f), 0.0014f * nCanvasWidth, false);

    std::ostringstream ssTime;
    ssTime.precision(2);
    ssTime << std::fixed << glfwGetTime();

    Renderer::DrawText(pScreenText, ssTime.str().c_str(),
                       glm::ivec2(nCanvasWidth / 32, nCanvasHeight / 16),
                       glm::vec3(1.0f, 1.0f, 1.0f), 0.0014f * nCanvasWidth, false);
}



void App::CheckForSpecialTiles()
{
    glm::vec2 vPlanarPos = glm::vec2(pPlayer->vPos.x, pPlayer->vPos.z);
    glm::vec2 vCurTile = glm::floor(vPlanarPos);

    int idx = vCurTile.y * pWorld->vWorldSize.x + vCurTile.x;

    if (pWorld->vLevelTiles[idx].eType == TileType::PORTAL_KEY)
    {
        pWorld->vLevelTiles[idx].eType = TileType::NORMAL;
        pWorld->vLevelTiles[idx].vCol = glm::vec3(0.2f, 0.2f, 0.5f);
        cShader.Use();
        cShader.SetBool("bKeyOn", 0);
        pPlayer->vPortalKeys.push_back(pWorld->vLevelTiles[idx].nPortalKey);
    }
    else if (pWorld->vLevelTiles[idx].eType == TileType::PORTAL)
    {
        if (CheckForIDMatch(pWorld->vLevelTiles[idx]))
        {
            pWorld->UnloadLevel();
            if (pWorld->nCurLevel < pWorld->nMaxLevel)
            {
                pWorld->LoadNextLevel(cShader);
                pPlayer->Spawn(pWorld->GetSpawnLoc());
            }
            else
            {
                pPlayer->nRunTime = (uint32_t)glfwGetTime();
                eState = GameState::WIN;
            }
        }
    }
    else if (eState == GameState::DEBUG)
    {
        pWorld->vLevelTiles[idx].vCol = glm::vec3(0.5f, 0.0f, 0.5f);
    }
}



bool App::CheckForIDMatch(TileInst &sTile)
{
    for (auto &nKeyID : pPlayer->vPortalKeys)
    {
        if (nKeyID == sTile.nID)
            return true;
    }

    return false;
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
    cShader.SetVec3("sDirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    cShader.SetVec3("sDirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    cShader.SetVec3("sDirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    cShader.SetVec3("sPlayerLight.position", glm::vec3(0.0f, 0.0f, 0.0f));
    cShader.SetVec3("sPlayerLight.ambient", glm::vec3(0.04f, 0.04f, 0.04f));
    cShader.SetVec3("sPlayerLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
    cShader.SetVec3("sPlayerLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
    cShader.SetFloat("sPlayerLight.constant", 0.2f);
    cShader.SetFloat("sPlayerLight.linear", 0.04f);
    cShader.SetFloat("sPlayerLight.quadratic", 0.064f);
    cShader.SetFloat("fPlayerLightScalar", 0.5f);


    cShader.SetVec3("sKeyLight.position", glm::vec3(8.0f, 0.0f, 8.0f));
    cShader.SetVec3("sKeyLight.ambient", glm::vec3(0.04f, 0.04f, 0.04f));
    cShader.SetVec3("sKeyLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
    cShader.SetVec3("sKeyLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
    cShader.SetFloat("sKeyLight.constant", 0.02f);
    cShader.SetFloat("sKeyLight.linear", 0.04f);
    cShader.SetFloat("sKeyLight.quadratic", 0.064f);
    cShader.SetBool("bKeyOn", 1);

    cShader.SetVec3("sInPortalLight.position", glm::vec3(8.0f, 1.0f, 8.0f));
    cShader.SetVec3("sInPortalLight.ambient", glm::vec3(0.04f, 0.04f, 0.04f));
    cShader.SetVec3("sInPortalLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
    cShader.SetVec3("sInPortalLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
    cShader.SetFloat("sInPortalLight.constant", 0.8f);
    cShader.SetFloat("sInPortalLight.linear", 0.04f);
    cShader.SetFloat("sInPortalLight.quadratic", 0.064f);
    cShader.SetBool("bInPortalOn", 1);

    cShader.SetVec3("sOutPortalLight.position", glm::vec3(8.0f, 0.0f, 8.0f));
    cShader.SetVec3("sOutPortalLight.ambient", glm::vec3(0.04f, 0.04f, 0.04f));
    cShader.SetVec3("sOutPortalLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
    cShader.SetVec3("sOutPortalLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
    cShader.SetFloat("sOutPortalLight.constant", 0.02f);
    cShader.SetFloat("sOutPortalLight.linear", 0.04f);
    cShader.SetFloat("sOutPortalLight.quadratic", 0.064f);
    cShader.SetBool("bOutPortalOn", 1);

    cShader.SetVec3("vFogColor", vFogColor);

}



void App::GLFWConfig()
{
    glfwSetFramebufferSizeCallback(pWindow, Framebuffer_Size_Callback);

    emscripten_set_mousemove_callback("#canvas", 0, 1, MouseCallback);
    emscripten_set_keydown_callback("#canvas", 0, 1, KeydownCallback);
    emscripten_set_keyup_callback("#canvas", 0, 1, KeyupCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void App::ProcessInput()
{
    switch(eState)
    {

        case GameState::RUNNING:
        {
            if (aKeyStates[(size_t)KEYS::P])
                pWorld->LoadNextLevel(cShader);

            if (aKeyStates[(size_t)KEYS::W])
                pPlayer->ProcessMovement(EntityMovement::FORWARD, fDeltaTime);
            if (aKeyStates[(size_t)KEYS::S])
                pPlayer->ProcessMovement(EntityMovement::BACKWARD, fDeltaTime);
            if (aKeyStates[(size_t)KEYS::A])
                pPlayer->ProcessMovement(EntityMovement::LEFT, fDeltaTime);
            if (aKeyStates[(size_t)KEYS::D])
                pPlayer->ProcessMovement(EntityMovement::RIGHT, fDeltaTime);

            break;
        }

        case GameState::DEBUG:
        {
            if (aKeyStates[(size_t)KEYS::W])
                cCamera.ProcessKeyboard(Camera_Movement::FORWARD, fDeltaTime, false);
            if (aKeyStates[(size_t)KEYS::S])
                cCamera.ProcessKeyboard(Camera_Movement::BACKWARD, fDeltaTime, false);
            if (aKeyStates[(size_t)KEYS::A])
                cCamera.ProcessKeyboard(Camera_Movement::LEFT, fDeltaTime, false);
            if (aKeyStates[(size_t)KEYS::D])
                cCamera.ProcessKeyboard(Camera_Movement::RIGHT, fDeltaTime, false);
        }
    }
}



void Framebuffer_Size_Callback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}



EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent *e, void* userData)
{
    if (eventType != EMSCRIPTEN_EVENT_MOUSEMOVE)
        return 0;

//    cCamera.ProcessMouseMovement(e->movementX, -e->movementY);

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

    switch(eState)
    {
        case GameState::MENU:
        {
            if (!strcmp(e->key, "Enter"))
            {

                if (!MiniAudioHighLevel(pMusic))
                {
                    std::cout << "ERROR::MINIAUDIO" << std::endl;
                }

                pWorld->nCurLevel = 0;
                pWorld = std::make_shared<World>();
                if (!pWorld->LoadNextLevel(cShader))
                    std::cout << "ERROR: Failed to load level" << std::endl;

                pPlayer = std::make_unique<Player>(pWorld, "/res/models/robot_01.obj");
                pPlayer->vPos = glm::vec3(pWorld->GetSpawnLoc()) + glm::vec3(0.5f, 0.0f, 0.5f);
                pPlayer->vCol = glm::vec3(0.1f, 0.3f, 0.3f);
                pPlayer->fShininess = 0.0f;

                eState = GameState::RUNNING;
                cCamera.bMenu = false;
                cCamera.vPos = glm::vec3(0.0f, 10.0f, 0.0f);
                glfwSetTime(0.0);
            }
            else if (!strcmp(e->key, "c"))
            {
                eState = GameState::CREDITS;
            }

            break;
        }

        case GameState::CREDITS:
        {
            if (!strcmp(e->key, "Enter"))
            {
                eState = GameState::MENU;
            }

            break;
        }

        case GameState::WIN:
        {
            if (!strcmp(e->key, "Enter"))
            {
                eState = GameState::MENU;
                cCamera.bMenu = true;
                cCamera.vPos = glm::vec3(-0.05f, 0.2f, 1.5f);
                cCamera.vUp = glm::vec3(0.0f, 1.0f, 0.0f);
                cCamera.fYaw = 270.0f;
                cCamera.fPitch = 0.0f;
                cCamera.UpdateCameraVectors();
            }

            break;
        }
    }

    if (!strcmp(e->key, "Enter"))
        aKeyStates[(size_t)KEYS::ENTER] = !aKeyStates[(size_t)KEYS::ENTER];

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



void MA_DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount)
{
    (void)pInput;

    ma_engine_read_pcm_frames((ma_engine*)pDevice->pUserData, pOutput, nFrameCount, NULL);
}



void MA_DataCallback_Decoder(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, nFrameCount, NULL);
}



bool MiniAudioHighLevel(const char* pPath)
{
    ma_result result;
    ma_device_config gDeviceConfig;

    ma_device_info* pPlaybackDeviceInfos;
    ma_uint32 nPlaybackDeviceCount;
    ma_device_info* pCaptureDeviceInfos;
    ma_uint32 nCaptureDeviceCount;
    ma_uint32 iDevice;

    if (ma_context_init(NULL, 0, NULL, &gContext) != MA_SUCCESS)\
    {
        std::cout << "ERROR::MINAUDIO: Failed to initialize context" << std::endl;
        return false;
    }

    result = ma_context_get_devices(&gContext, &pPlaybackDeviceInfos, &nPlaybackDeviceCount, &pCaptureDeviceInfos, &nCaptureDeviceCount);

    if (ma_context_init(NULL, 0, NULL, &gContext) != MA_SUCCESS)\
    {
        std::cout << "ERROR::MINAUDIO: Failed to retrieve device information" << std::endl;
        return false;
    }

    std::cout << "Playback Devices:" << std::endl;
    for (iDevice = 0; iDevice < nPlaybackDeviceCount; iDevice++)
    {
        std::cout << "    " << iDevice << ": " << pPlaybackDeviceInfos[iDevice].name << std::endl;
    }

    std::cout << "Capture Devices:" << std::endl;
    for (iDevice = 0; iDevice < nPlaybackDeviceCount; iDevice++)
    {
        std::cout << "    " << iDevice << ": " << pCaptureDeviceInfos[iDevice].name << std::endl;
    }


    result = ma_decoder_init_file(pPath, NULL, &gDecoder);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR::MINAUDIO: Could not load audio file" << std::endl;
        return false;
    }

    gDeviceConfig = ma_device_config_init(ma_device_type_playback);
    gDeviceConfig.playback.format   = gDecoder.outputFormat;
    gDeviceConfig.playback.channels = gDecoder.outputChannels;
    gDeviceConfig.sampleRate        = gDecoder.outputSampleRate;
    gDeviceConfig.dataCallback      = MA_DataCallback_Decoder;
    gDeviceConfig.pUserData         = &gDecoder;

    if (ma_device_init(NULL, &gDeviceConfig, &gDevice) != MA_SUCCESS)
    {
        std::cout << "ERROR::MINAUDIO: Failed to open playback device" << std::endl;
        ma_decoder_uninit(&gDecoder);
        return false;
    }

    if (ma_device_start(&gDevice) != MA_SUCCESS)
    {
        std::cout << "ERROR::MINAUDIO: Failed to start playback device" << std::endl;
        ma_device_uninit(&gDevice);
        ma_decoder_uninit(&gDecoder);
        return false;
    }

    return true;
}



bool MiniAudioLowLevel(const char* pPath)
{
    ma_result result;
    ma_device_config gDeviceConfig;
    ma_engine_config gEngineConfig;

    gDeviceConfig = ma_device_config_init(ma_device_type_playback);
    gDeviceConfig.playback.format   = ma_format_unknown;
    gDeviceConfig.playback.channels = CHANNELS;
    gDeviceConfig.sampleRate        = SAMPLE_RATE;
    gDeviceConfig.dataCallback      = MA_DataCallback;

    gEngineConfig = ma_engine_config_init();
    gEngineConfig.pDevice    = &gDevice;

    if (ma_device_init(NULL, &gDeviceConfig, &gDevice) != MA_SUCCESS)
    {
        std::cout << "ERROR::MINIAUDIO: Failed to open playback device" << std::endl;
        return false;
    }

    if (ma_device_start(&gDevice) != MA_SUCCESS)
    {
        std::cout << "ERROR::MINIAUDIO: Failed to start playback device" << std::endl;
        return false;
    }

    result = ma_engine_init(&gEngineConfig, &gEngine);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR::MINIAUDIO: Failed to initialize audio engine." << std::endl;
        return false;
    }

    result = ma_engine_start(&gEngine);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR::MINIAUDIO: Failed to start audio engine." << std::endl;
        return false;
    }

    result = ma_sound_init_from_file(&gEngine, pPath, 0, NULL, NULL, &gSound);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR::MINIAUDIO: Failed to initialize sound." << std::endl;
        return false;
    }

    ma_sound_set_looping(&gSound, MA_TRUE);

    result = ma_sound_start(&gSound);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR::MINIAUDIO: Failed to start sound." << std::endl;
        return false;
    }

    return true;
}



float RandFloatInRange(float fMin, float fMax)
{
    uint32_t nMin = (uint32_t)(fMin * 10000);
    uint32_t nMax = (uint32_t)(fMax * 10000);

    return (rand() % (nMax + 1 - nMin) + nMin) / 10000.0f;
}
