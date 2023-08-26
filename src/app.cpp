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

    CompileShaders();

    load_geo(vertices, indices);

    std::cout << "Create" << std::endl;
}


void App::Update()
{
    processInput(pWindow);
    Renderer::Draw(vao, shader);

    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}



void App::load_geo(float* vertices, unsigned int* indices)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}



void App::CompileShaders()
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    // check for linking errors
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader
                , 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
