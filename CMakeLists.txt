cmake_minimum_required(VERSION 3.24)
project(light)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

#include important library

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

# set source file and dependent file.

set(ARGORITHM_SOURCE)
set(HANDLE_GRAPHIC_SOURCE)
set(VOLTEX_SOURCE)
set(ENTITY_SOURCE)
set(CUBE_SOURCE)
set(UTIL_SOURCE)
set(CXX_FILE src/main.cpp)
set(DEP_FILE dependences/src/stb/stb.cpp)

# file(GLOB ARGORITHM_SOURCE CONFIGURE_DEPENDS "src/argorithm/src/*.cpp")
file(GLOB HANDLE_GRAPHIC_SOURCE CONFIGURE_DEPENDS "src/handle_graphic/src/*.cpp")
file(GLOB ENTITY_SOURCE CONFIGURE_DEPENDS "src/entity/src/*.cpp")
file(GLOB VOLTEX_SOURCE CONFIGURE_DEPENDS "src/votex/src/*.cpp")
file(GLOB UTIL_SOURCE CONFIGURE_DEPENDS "src/util/src/*.cpp")
#file(GLOB CUBE_SOURCE CONFIGURE_DEPENDS "cube/src/*.cpp")
list (APPEND CXX_FILE 
        ${ARGORITHM_SOURCE}
        ${HANDLE_GRAPHIC_SOURCE} 
        ${CUBE_SOURCE}
        ${VOLTEX_SOURCE}
        ${UTIL_SOURCE})

set_source_files_properties(${CXX_FILE}
        PROPERTIES 
        COMPILE_FLAGS 
        "-O2 -DNDEBUG -Wall -Wextra")


include_directories(${PROJECT_NAME} 
        dependences/include/stb 
        dependences/include/imgui 
        dependences/include/glm 
        dependences 
        src/argorithm 
        src/handle_graphic
        src/votex
        src/util
        src/entity
        ./)

add_executable(${PROJECT_NAME} ${CXX_FILE} ${DEP_FILE})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
    set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
    target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static GLEW::glew_s OpenGL::GL)
endif()

if ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
    target_link_libraries(${PROJECT_NAME} SDL2::SDL2 GLEW::GLEW OpenGL::GL)
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND bash -c "cp -ruv ../resources ../build"
    VERBATIM
)