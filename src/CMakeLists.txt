cmake_minimum_required(VERSION 2.8)

# flooid source
set(FLOOID_SOURCE
        main.cpp
        ParticleManager.cpp
        PBDSolver.cpp
        utils.cpp
        PBFRenderer.cpp
        Application.cpp
        Camera.cpp

        scene/Scene.cpp
        scene/Mesh.cpp
        scene/Plane.cpp

        shader/Shader.cpp)

# flooid header
set(FLOOID_HEADER
        PBDSolver.h
        ParticleManager.h
        particle.h
        kernels.h
        utils.h
        common.h
        PBFRenderer.h
        Application.h
        Camera.h

        scene/Scene.h
        scene/Mesh.h
        scene/Plane.h

        shader/Shader.h

        AppConfig.h)


#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${eng_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
)

#-------------------------------------------------------------------------------
# Set link directories
#-------------------------------------------------------------------------------
link_directories(
        ${GLEW_LIBRARY_DIRS}
        ${GLFW_LIBRARY_DIRS}
        ${eng_LIBRARY_DIRS}
        ${FREETYPE_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Add executable
#-------------------------------------------------------------------------------
add_executable(flooid
        ${FLOOID_SOURCE}
        ${FLOOID_HEADER}
        )

target_link_libraries(flooid
        eng ${eng_LIBRARIES}
        glew ${GLEW_LIBRARIES}
        glfw ${GLFW_LIBRARIES}
        ${OPengL_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        )

#-------------------------------------------------------------------------------
# Platform-specific configurations for target
#-------------------------------------------------------------------------------
if(APPLE)
    set_property(TARGET flooid APPEND_STRING PROPERTY COMPILE_FLAGS
            "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)


# Put executable in build directory root
set(EXECUTABLE_OUTPUT_PATH ..)

# Install to project root
install(TARGETS flooid DESTINATION ${flooid_SOURCE_DIR})
