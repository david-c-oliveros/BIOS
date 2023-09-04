cmake_minimum_required(VERSION 2.23.3)

add_compile_options(-Wfatal-errors -w -O2 -D WITH_MINIAUDIO -pthread)

find_package(OpenGL)

project(App)

set(BOX_2D_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../external/box2d)

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
    src/box_collider.cpp
    src/shader.cpp
    src/world.cpp
    src/player.cpp
    src/olcPixelGameEngine.cpp
    )

#add_subdirectory(external/glfw)
add_subdirectory(external/assimp)

include_directories(
    external/glfw/include)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glad
    PUBLIC external/glText
    PUBLIC external/stb
    PUBLIC external/glm
    PUBLIC external/box2d/include
    PUBLIC external/assimp/include
    PUBLIC external/olcPixelGameEngine
    PUBLIC external/olcPixelGameEngine/extensions
    PUBLIC src/soloud/include
    )

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    PRIVATE external/box2d/build/bin
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
    -s WASM=1 \
    -s EXPORT_ALL=1 \
    -s MALLOC=emmalloc \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MIN_WEBGL_VERSION=2 \
    -s USE_LIBPNG=1 \
    -s USE_WEBGL2=1 \
    -s USE_GLFW=3 \
    --preload-file res \
    --preload-file shaders")
