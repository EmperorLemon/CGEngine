cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME CGCore)
project(${PROJECT_NAME} LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

add_library(${PROJECT_NAME} STATIC ${ENGINE_SRC})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(CG_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(CG_RELEASE)
endif()

add_subdirectory(ThirdParty)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_custom_command(TARGET CGCore POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                   ${CMAKE_CURRENT_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:CGCore>/Assets)