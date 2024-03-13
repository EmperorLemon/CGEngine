project(imguizmo)

add_library(imguizmo STATIC)

target_include_directories(imguizmo SYSTEM PUBLIC
    ImGuizmo/
)

file(GLOB SRC_IMGUIZMO ImGuizmo/*.cpp)
target_sources(imguizmo PRIVATE ${SRC_IMGUIZMO})