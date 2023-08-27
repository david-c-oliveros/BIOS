#include "renderer.h"



Renderer::Renderer(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight)
    : nCanvasWidth(_nCanvasWidth), nCanvasHeight(_nCanvasHeight)
{
}



Renderer::~Renderer()
{
}



void Renderer::Clear(glm::vec4 vClearColor)
{
    glClearColor(vClearColor.x, vClearColor.y, vClearColor.z, vClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
}



void Renderer::Draw(GLuint &vao, GLuint &vbo, GLuint shader, GLuint nNumVert)
{

    glUseProgram(shader);

    emscripten_glBindVertexArrayOES(vao);

    glDrawElements(GL_TRIANGLES, nNumVert, GL_UNSIGNED_INT, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glDrawArrays(GL_TRIANGLES, 0, 3);
}



bool Renderer::Init_GLFW(GLFWwindow* pWindow, uint32_t nWidth, uint32_t nHeight)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(nWidth, nHeight, "LearnOpenGL", NULL, NULL);
    if (pWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(pWindow);

    std::cout << "Init GLFW" << std::endl;

    return true;
}



bool Renderer::Init_WebGL(uint32_t nWidth, uint32_t nHeight, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE &glContext, EmscriptenWebGLContextAttributes &attrs)
{
    double dpr = emscripten_get_device_pixel_ratio();
    emscripten_set_element_css_size("#canvas", nWidth / dpr, nHeight / dpr);
    emscripten_set_canvas_element_size("#canvas", nWidth, nHeight);

    emscripten_webgl_init_context_attributes(&attrs);
    attrs.alpha = 0;

    attrs.majorVersion = 2;

    glContext = emscripten_webgl_create_context("#canvas", &attrs);
    assert(glContext);
    emscripten_webgl_make_context_current(glContext);

    //assert(emscripten_webgl_enable_OES_vertex_array_object(glContext));

    std::cout << "Init WebGL" << std::endl;

    return true;
}
