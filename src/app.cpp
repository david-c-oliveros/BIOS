#include "app.h"



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

    Renderer::Init_GLFW(pWindow, nCanvasWidth, nCanvasHeight, Framebuffer_Size_Callback);
    Renderer::Init_WebGL(nCanvasWidth, nCanvasHeight, attrs);

    LoadShaders();

    cCube = std::make_unique<Object>("/res/cube.obj");
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

    cCube->Draw(cShader);
}



//void App::load_geo(float* vertices, unsigned int* indices)
//{
//    std::cout << "load_geo\n";
//    // TODO - Finish setting up vao
////    glGenVertexArrays(1, &vao);
////    glBindVertexArray(vao);
//
//    glGenBuffers(1, &vbo);
//    glGenBuffers(1, &ebo);
//
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices), indices, GL_STATIC_DRAW);
//
////    GLuint aPosLoc = glGetAttribLocation(shader, "aPos");
////    std::cout << "Attribute location: " << aPosLoc << std::endl;
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
////    glBindBuffer(GL_ARRAY_BUFFER, 0); 
//}



void App::LoadShaders()
{
    cShader.Create("shaders/color_vs.shader", "shaders/color_fs.shader");
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
