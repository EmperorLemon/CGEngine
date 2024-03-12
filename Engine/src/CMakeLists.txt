add_subdirectory(Core)
add_subdirectory(ECS)
add_subdirectory(GUI)
add_subdirectory(IO)
add_subdirectory(Math)
add_subdirectory(Platform)
add_subdirectory(Renderer)
add_subdirectory(Scene)
add_subdirectory(Utils)

set(ENGINE_SRC

${CORE}
${ECS}
${GUI}
${IO}
${MATH}
${PLATFORM}
${RENDERER}
${SCENE}
${UTILS}

${CMAKE_CURRENT_SOURCE_DIR}/CGEngine.h

PARENT_SCOPE)