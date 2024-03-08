set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)

set(BUILD_DOC OFF CACHE INTERNAL "" FORCE)
set(BUILD_SANDBOX OFF CACHE INTERNAL "" FORCE)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE INTERNAL "" FORCE)

add_subdirectory(crossguid)
add_subdirectory(entt INTERFACE)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(tinygltf)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    crossguid/include
    entt/src
    glad/include
    glfw/include
    glm
    spdlog/include
    stb/
)