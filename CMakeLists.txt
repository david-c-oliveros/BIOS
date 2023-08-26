cmake_minimum_required(VERSION 2.23.3)

add_compile_options(-Wfatal-errors -w -O2)
#add_definitions("-s ALLOW_MEMORY_GROWTH=1 -s ALLOW_MEMORY_GROWTH=1 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_GLFW=3")

find_package(OpenGL)

project(App)

add_executable(${PROJECT_NAME}
    external/glad/glad.c
    src/main.cpp
    src/app.cpp
    src/renderer.cpp
    )

#add_subdirectory(external/glfw)
#add_subdirectory(external/X11)

include_directories(external/glfw/include)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glad
    )

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    )

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES}
    m
    dl
    )

set_target_properties(${PROJECT_NAME}
    PROPERTIES SUFFIX ".html"
    LINK_FLAGS " --bind --emrun -s FULL_ES3=1 -s WASM=1 -s EXPORT_ALL=1 -s ALLOW_MEMORY_GROWTH=1 -s ALLOW_MEMORY_GROWTH=1 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_WEBGL2=1 -s USE_GLFW=3")
