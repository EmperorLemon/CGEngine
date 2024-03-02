set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(spdlog)
add_subdirectory(glad)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    glfw/include
    spdlog/include
    glad/include
)