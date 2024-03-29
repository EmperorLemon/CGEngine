set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(SET_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE INTERNAL "" FORCE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)

set(BUILD_DOC OFF CACHE INTERNAL "" FORCE)
set(BUILD_SANDBOX OFF CACHE INTERNAL "" FORCE)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE INTERNAL "" FORCE)

add_subdirectory(assimp)
add_subdirectory(crossguid)
add_subdirectory(entt INTERFACE)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(imguizmo)
add_subdirectory(spdlog)
add_subdirectory(tinygltf)

target_include_directories(imgui PUBLIC glfw/include)
target_include_directories(imguizmo PUBLIC imgui/imgui)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    assimp/include
    crossguid/include
    entt/src
    glad/include
    glfw/include
    glm
    imguizmo/
    spdlog/include
    stb/
)