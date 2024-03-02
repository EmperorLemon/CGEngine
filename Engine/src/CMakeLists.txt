add_subdirectory(Core)
add_subdirectory(Platform)
add_subdirectory(Renderer)

set(ENGINE_SRC

${CORE}
${PLATFORM}
${RENDERER}

${CMAKE_CURRENT_SOURCE_DIR}/CGEngine.h

PARENT_SCOPE)