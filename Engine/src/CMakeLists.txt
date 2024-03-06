add_subdirectory(Component)
add_subdirectory(Core)
add_subdirectory(IO)
add_subdirectory(Platform)
add_subdirectory(Renderer)

set(ENGINE_SRC

${COMPONENT}
${CORE}
${IO}
${PLATFORM}
${RENDERER}

${CMAKE_CURRENT_SOURCE_DIR}/CGEngine.h

PARENT_SCOPE)