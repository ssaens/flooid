cmake_minimum_required(VERSION 3.6)

# flooid source
set(FLOOID2_SOURCE
        main.cpp

        display/Application.cpp

        model/Mesh.cpp
        model/Model.cpp

        PBD/ParticleManager.cpp
        PBD/PBDSolver.cpp)

# flooid header
set(FLOOID2_HEADER

        display/Application.h
        display/Camera.h

        model/Shader.h
        model/Mesh.h
        model/Model.h

        PBD/Particle.h
        PBD/ParticleManager.h
        PBD/PBDSolver.h)


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
add_executable(flooid2
        ${FLOOID2_SOURCE}
        ${FLOOID2_HEADER}
        )

target_link_libraries(flooid2
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
    set_property(TARGET flooid2 APPEND_STRING PROPERTY COMPILE_FLAGS
            "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)


# Put executable in build directory root
set(EXECUTABLE_OUTPUT_PATH ..)

# Install to project root
install(TARGETS flooid2 DESTINATION ${flooid2_SOURCE_DIR})
