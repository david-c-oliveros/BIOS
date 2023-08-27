#include "app.h"



Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float fLast_X = 0.0f;
float fLast_Y = 0.0f;

bool bFirstMouse = true;



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
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    Renderer::Init_GLFW(pWindow, nCanvasWidth, nCanvasHeight);
    Renderer::Init_WebGL(nCanvasWidth, nCanvasHeight, glContext, attrs);
    GLFWConfig();

    LoadShaders();

    cCube = std::make_unique<Object>("/res/tile.obj", glm::vec3(0.0f, 0.0f, 0.0f));
//    load_geo(vertices, indices);
}


void App::Update()
{
    processInput(pWindow);

    Render();

    //glfwSwapBuffers(pWindow);
    glfwPollEvents();
}



void App::Render()
{
    Renderer::Clear(glm::vec4(0.1f, 0.1f, 0.3f, 1.0f));

    glm::mat4 mView = camera.GetViewMatrix();
    glm::mat4 mProjection = glm::perspective(glm::radians(camera.fZoom), (float)nCanvasWidth / (float)nCanvasHeight,  0.1f, 1000.0f);

    cShader.SetMat4("mView", mView);
    cShader.SetMat4("mProjection", mProjection);

    cCube->fRotAngle = 100 * glfwGetTime();
    cCube->Draw(cShader);
}



void App::LoadShaders()
{
    cShader.Create("shaders/multiple_lights_vs.shader", "shaders/multiple_lights_fs.shader");
}



void App::GLFWConfig()
{
    glfwSetFramebufferSizeCallback(pWindow, Framebuffer_Size_Callback);
//    glfwSetCursorPosCallback(pWindow, MouseCallback);

    emscripten_set_mousemove_callback("#canvas", 0, 1, MouseCallback);
    emscripten_set_keypress_callback("#canvas", this, 1, KeydownCallback);
}



void processInput(GLFWwindow *pWindow)
{
    if(glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
        emscripten_cancel_main_loop();
    }
}



void Framebuffer_Size_Callback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}



EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent *e, void* userData)
{
    float xpos = static_cast<float>(e->screenX);
    float ypos = static_cast<float>(e->screenY);

    std::cout << "mouse: " << xpos << ", " << ypos << std::endl;

    if (bFirstMouse)
    {
        fLast_X = xpos;
        fLast_Y = xpos;
        bFirstMouse = false;
    }

    float xoffset = xpos - fLast_X;
    float yoffset = fLast_Y - ypos;

    fLast_X = xpos;
    fLast_Y = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

    return 0;
}



EM_BOOL KeydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
{
    if (!strcmp(e->key, "Enter"))
        emscripten_request_pointerlock("#canvas", 1);

    if (!strcmp(e->key, "w"))
        camera.ProcessKeyboard(Camera_Movement::FORWARD, 0.1, false);
    if (!strcmp(e->key, "s"))
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.1, false);
    if (!strcmp(e->key, "a"))
        camera.ProcessKeyboard(Camera_Movement::LEFT, 0.1, false);
    if (!strcmp(e->key, "d"))
        camera.ProcessKeyboard(Camera_Movement::RIGHT, 0.1, false);

    std::cout << "Camera position: " << glm::to_string(camera.vPos) << std::endl;
    std::cout << "Camera direciton: " << glm::to_string(camera.Front) << std::endl;

    return 0;
}



EM_BOOL PointerlockChangeCallback(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData)
{
    std::cout << "pointerlock change callback" << std::endl;
    emscripten_request_pointerlock("#canvas", 1);

    return 0;
}
