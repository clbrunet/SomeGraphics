set(GLFW_BUILD_EXAMPLES Off)
set(GLFW_BUILD_TESTS Off)
set(GLFW_BUILD_DOCS Off)
add_subdirectory("./ext/glfw")

add_subdirectory("./ext/glad")

file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(SomeGraphicsEngine STATIC ${SRCS})

target_compile_definitions(SomeGraphicsEngine PUBLIC
  GLFW_INCLUDE_NONE
  $<$<CONFIG:Debug>:SG_DEBUG=1>
  )
target_compile_options(SomeGraphicsEngine PRIVATE
  "-Wall"
  "-Wextra"
  "-Werror"
  $<$<CONFIG:Debug>:-g3>
  )
target_include_directories(SomeGraphicsEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(SomeGraphicsEngine PUBLIC
  glfw
  glad
  )
