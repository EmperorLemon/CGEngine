add_subdirectory(Core)
add_subdirectory(ECS)
add_subdirectory(IO)
add_subdirectory(Math)
add_subdirectory(Platform)
add_subdirectory(Renderer)

set(ENGINE_SRC

${CORE}
${ECS}
${IO}
${MATH}
${PLATFORM}
${RENDERER}

${CMAKE_CURRENT_SOURCE_DIR}/CGEngine.h

PARENT_SCOPE)