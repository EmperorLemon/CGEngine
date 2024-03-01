add_subdirectory(Core)
add_subdirectory(Renderer)

set(ENGINE_SRC

${CORE}
${RENDERER}

${CMAKE_CURRENT_SOURCE_DIR}/CGEngine.h
${CMAKE_CURRENT_SOURCE_DIR}/cgpch.h

PARENT_SCOPE)