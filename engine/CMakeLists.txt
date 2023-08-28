set(GLFW_BUILD_EXAMPLES Off)
set(GLFW_BUILD_TESTS Off)
set(GLFW_BUILD_DOCS Off)
add_subdirectory("./ext/glfw" SYSTEM)

add_subdirectory("./ext/glad" SYSTEM)

add_subdirectory("./ext/glm" SYSTEM)

add_subdirectory("./ext/stb" SYSTEM)

add_subdirectory("./ext/imgui" SYSTEM)

set(CMAKE_POLICY_DEFAULT_CMP0077 "NEW")
set(BUILD_SHARED_LIBS Off)
set(ASSIMP_NO_EXPORT On)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT Off)
set(ASSIMP_BUILD_TESTS Off)
set(ASSIMP_INSTALL Off)
set(ASSIMP_BUILD_GLTF_IMPORTER On)
add_subdirectory("./ext/assimp" SYSTEM)

file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(SomeGraphicsEngine STATIC ${SRCS})

target_compile_definitions(SomeGraphicsEngine PUBLIC
  GLFW_INCLUDE_NONE
  $<$<CONFIG:Debug>:SG_DEBUG=1>
  )
if(CMAKE_HOST_WIN32)
  target_compile_options(SomeGraphicsEngine PRIVATE "-Wall")
else()
  target_compile_options(SomeGraphicsEngine PRIVATE
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    $<$<CONFIG:Debug>:-g3>
    )
endif()
target_include_directories(SomeGraphicsEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(SomeGraphicsEngine PUBLIC
  glfw
  glad
  glm
  stb
  imgui
  assimp
  )
