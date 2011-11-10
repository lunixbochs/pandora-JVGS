# Set up include directories.
INCLUDE_DIRECTORIES(
    ${SDL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${OPENGLES_INCLUDE_DIR}
    ${EGL_INCLUDE_DIR}
)

ADD_LIBRARY( 
    video STATIC 
    Color.cpp
    DataRenderer.cpp
    ListManager.cpp
    DisplayListWrapper.cpp
    FPSLogger.cpp
    Renderer.cpp
    SketchyRenderer.cpp
    VideoManager.cpp
    eglport.c
)

# Link to the following libraries
TARGET_LINK_LIBRARIES(
    video
    math
    ${SDL_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${OPENGLES_LIBRARIES}
    ${EGL_LIBRARIES}
)
