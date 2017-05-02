cmake_minimum_required(VERSION 2.8)

# flooid source
set(FLOOID2_SOURCE
        main.cpp
        util.cpp

        display/Application.cpp

        scene/Mesh.cpp
        scene/Model.cpp

        scene/Scene.cpp
        Skybox/Skybox.cpp)

# flooid header
set(FLOOID2_HEADER
        common.h
        util.h

        display/Application.h
        display/Camera.h

        scene/Shader.h
        scene/Mesh.h
        scene/Model.h

        PBD/Particle.h
        PBD/ParticleManager.h

        scene/Light.h
        scene/Scene.h
        scene/Plane.h
        scene/triangle.h
        Skybox/Skybox.h)

set(CUDA_SOURCE_FILES
    PBD/ParticleManager.cu
    cuda/CudaPBDSolver.cu
    cuda/CudaPBDSolver.cuh
)

set(CUDA_NVCC_FLAGS
        ${CUDA_NVCC_FLAGS}
        -std=c++11
        -O3 -gencode arch=compute_30,code=sm_30)

set(CUDA_SEPARABLE_COMPILATION ON)
set(CUDA_PROPAGATE_HOST_FLAGS OFF)
set(CUDA_HOST_COMPILER g++)
set(CUDA_VERBOSE_BUILD OFF)

cuda_add_executable(flooid2
    ${FLOOID2_SOURCE}
    ${FLOOID2_HEADER}
    ${CUDA_SOURCE_FILES}
)

#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
        ${CUDA_INCLUDE_DIRS}
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
#add_executable(flooid2
#        
#)

target_link_libraries(flooid2
        eng ${eng_LIBRARIES}
        glew ${GLEW_LIBRARIES}
        glfw ${GLFW_LIBRARIES}
        ${OPENGL_LIBRARIES}
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
