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
    #    src/soloud/core/soloud.cpp
    #    src/soloud/core/soloud_audiosource.cpp
    #    src/soloud/core/soloud_bus.cpp
    #    src/soloud/core/soloud_core_3d.cpp
    #    src/soloud/core/soloud_core_basicops.cpp
    #    src/soloud/core/soloud_core_faderops.cpp
    #    src/soloud/core/soloud_core_getters.cpp
    #    src/soloud/core/soloud_core_setters.cpp
    #    src/soloud/core/soloud_core_voicegroup.cpp
    #    src/soloud/core/soloud_core_voiceops.cpp
    #    src/soloud/core/soloud_fader.cpp
    #    src/soloud/core/soloud_fft.cpp
    #    src/soloud/core/soloud_fft_lut.cpp
    #    src/soloud/core/soloud_file.cpp
    #    src/soloud/core/soloud_filter.cpp
    #    src/soloud/core/soloud_misc.cpp
    #    src/soloud/core/soloud_queue.cpp
    #    src/soloud/core/soloud_thread.cpp
    #    src/soloud/miniaudio/soloud_miniaudio.cpp
    #    src/soloud/wav/dr_impl.cpp
    #    src/soloud/wav/soloud_wav.cpp
    #    src/soloud/wav/soloud_wavstream.cpp
    #    src/soloud/wav/stb_vorbis.c
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
#LINK_FLAGS " --bind --emrun -sUSE_PTHREADS -DMA_ENABLE_AUDIO_WORKLETS -sAUDIO_WORKLET=1 -sASYNCIFY -sWASM_WORKERS=1 -sFORCE_FILESYSTEM -sFULL_ES3=1 -sWASM=1 -sEXPORT_ALL=1 -sALLOW_MEMORY_GROWTH=1 -sALLOW_MEMORY_GROWTH=1 -sMIN_WEBGL_VERSION=2 -sUSE_LIBPNG=1 -sUSE_WEBGL2=1 -sUSE_GLFW=3 --preload-file res --preload-file shaders")
