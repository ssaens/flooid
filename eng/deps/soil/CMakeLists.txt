set(SOIL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOIL_SOURCE_DIR ${SOIL_ROOT_DIR}/src)
set(SOIL_HEADER_DIR ${SOIL_ROOT_DIR}/include)

include_directories(${SOIL_HEADER_DIR})
add_library(soil
        ${SOIL_SOURCE_DIR}/image_DXT.c
        ${SOIL_SOURCE_DIR}/image_helper.c
        ${SOIL_SOURCE_DIR}/SOIL.c
        ${SOIL_SOURCE_DIR}/stb_image_aug.c)

set(SOIL_LIBRARIES soil PARENT_SCOPE)
set(SOIL_INCLUDE_DIRS ${SOIL_HEADER_DIR} PARENT_SCOPE)

install(TARGETS soil DESTINATION lib)
install(DIRECTORY ${SOIL_HEADER_DIR}/soil DESTINATION include)