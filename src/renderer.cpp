#define GLT_IMPLEMENTATION
#define GLT_DEBUG_PRINT

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void Renderer::Draw(GLuint &vao, GLuint &vbo, GLuint shaderID, GLuint nNumVert)
{

    glUseProgram(shaderID);

    emscripten_glBindVertexArrayOES(vao);

    glDrawElements(GL_TRIANGLES, nNumVert, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}



void Renderer::DrawText(GLTtext* pScreenText, const char* pStr, glm::ivec2 vPos, glm::vec3 vColor, float fScale, bool bAligned)
{
    gltBeginDraw();
    gltSetText(pScreenText, pStr);
    gltColor(vColor.x, vColor.y, vColor.z, 1.0f);

    if (bAligned)
    {
        gltDrawText2DAligned(pScreenText,
                             (GLfloat)vPos.x,
                             (GLfloat)vPos.y,
                             fScale,
                             GLT_CENTER, GLT_CENTER);
    }
    else
    {
        gltDrawText2D(pScreenText, (GLfloat)vPos.x, (GLfloat)vPos.y, fScale);
    }

    gltEndDraw();
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

    return true;
}



bool Renderer::Init_GLText()
{
    if (!gltInit())
    {
        std::cout << "\nERROR: Failed to initialize glText" << std::endl;
        return false;
    }

    return true;
}
