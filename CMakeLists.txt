cmake_minimum_required(VERSION 2.23.3)

add_compile_options(-Wfatal-errors -w -pthread)

find_package(OpenGL)

project(App)

add_executable(${PROJECT_NAME}
    external/glad/glad.c
    src/stb_image.cpp
    src/main.cpp
    src/app.cpp
    src/renderer.cpp
    src/camera.cpp
    src/mesh.cpp
    src/model.cpp
    src/object.cpp
    src/shader.cpp
    src/world.cpp
    src/player.cpp
    src/audio.cpp
    )

#add_subdirectory(external/glfw)
add_subdirectory(external/assimp)

include_directories(
    external/glfw/include)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glad
    PUBLIC external/glText
    PUBLIC external/glm
    PUBLIC external/minaudio
    PUBLIC external/assimp/include
    )

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    PRIVATE external/assimp/bin
    )

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES}
    m
    dl
    pthread
    assimp
    )

set_target_properties(${PROJECT_NAME}
    PROPERTIES SUFFIX ".html"
    LINK_FLAGS " --bind --emrun \
    -s FORCE_FILESYSTEM \
    -s FULL_ES3=1 \
    -s EXPORT_ALL=1 \
    -s MALLOC=emmalloc \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MIN_WEBGL_VERSION=2 \
    -s USE_LIBPNG=1 \
    -s USE_WEBGL2=1 \
    -s USE_GLFW=3 \
    --preload-file ../../res \
    --preload-file ../../shaders")
