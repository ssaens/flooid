cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# eng library source files
set(eng_SOURCE
        View.cpp
        )

# eng library header files
set(eng_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include/eng)
set(eng_HEADER
        ${eng_HEADER_DIR}/eng.h
        ${eng_HEADER_DIR}/View.h
        ${eng_HEADER_DIR}/Renderer.h
        )

#-------------------------------------------------------------------------------
# Building static library (always)
#-------------------------------------------------------------------------------
add_library(eng STATIC ${eng_SOURCE} ${eng_HEADER})

target_link_libraries(
        eng
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${FREETYPE_LIBRARIES}
)

#-------------------------------------------------------------------------------
# Building shared library
#-------------------------------------------------------------------------------
if(eng_BUILD_SHARED)
    add_library(eng_SHARED SHARED ${eng_SOURCE} ${eng_HEADER})
    target_link_libraries(
            eng_SHARED
            ${GLEW_LIBRARIES}
            ${GLFW_LIBRARIES}
            ${OPENGL_LIBRARIES}
            ${FREETYPE_LIBRARIES}
    )
endif()

#-------------------------------------------------------------------------------
# Platform-specific target configurations
#-------------------------------------------------------------------------------

###################
# Building on OSX #
###################
if (APPLE)

    # Clang options
    target_compile_options(eng PRIVATE -Wno-constant-conversion)

    # Framework dependencies
    target_link_libraries(eng
            ${COCOA_LIBRARIES}
            ${IOKIT_LIBRARIES}
            ${COREVIDEO_LIBRARIES})

    # Output name
    if(eng_BUILD_DEBUG)
        set_target_properties(eng PROPERTIES OUTPUT_NAME eng_osx_d)
        if(eng_BUILD_SHARED)
            set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng_osx_d)
        endif()
    else(eng_BUILD_DEBUG)
        set_target_properties(eng PROPERTIES OUTPUT_NAME eng_osx)
        if(eng_BUILD_SHARED)
            set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng_osx)
        endif()
    endif(eng_BUILD_DEBUG)

endif(APPLE)

##################
# Build on Linux #
##################
set(LINUX UNIX AND NOT APPLE)
if(LINUX)
    if(eng_BUILD_DEBUG)
        set_target_properties(eng PROPERTIES OUTPUT_NAME eng_d)
        if(eng_BUILD_SHARED)
            set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng_d)
        endif()
    else(eng_BUILD_DEBUG)
        set_target_properties(eng PROPERTIES OUTPUT_NAME eng)
        if(eng_BUILD_SHARED)
            set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng)
        endif()
    endif(eng_BUILD_DEBUG)
endif(LINUX)

####################
# Build on Windows #
####################
if(WIN32)

    if(MSVC)
        if(eng_BUILD_DEBUG)
            set_target_properties(eng PROPERTIES OUTPUT_NAME eng_d)
            if(eng_BUILD_SHARED)
                set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng_d)
            endif()
        else(eng_BUILD_DEBUG)
            set_target_properties(eng PROPERTIES OUTPUT_NAME eng)
            if(eng_BUILD_SHARED)
                set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng)
            endif()
        endif(eng_BUILD_DEBUG)
    endif(MSVC)

    if(MINGW)
        if(eng_BUILD_DEBUG)
            set_target_properties(eng PROPERTIES OUTPUT_NAME eng_d)
            if(eng_BUILD_SHARED)
                set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng_d)
            endif()
        else(eng_BUILD_DEBUG)
            set_target_properties(eng PROPERTIES OUTPUT_NAME eng)
            if(eng_BUILD_SHARED)
                set_target_properties(eng_SHARED PROPERTIES OUTPUT_NAME eng)
            endif()
        endif(eng_BUILD_DEBUG)
    endif(MINGW)

endif(WIN32)

#-------------------------------------------------------------------------------
# Installation configurations
#-------------------------------------------------------------------------------
install(TARGETS eng DESTINATION lib)
if(eng_BUILD_SHARED)
    install(TARGETS eng_SHARED DESTINATION lib)
endif()

# install headers
install(FILES ${eng_HEADER} DESTINATION include/eng)
