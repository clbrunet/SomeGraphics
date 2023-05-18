file(GLOB_RECURSE SRCS "src/*.cpp")

set(GLFW_BUILD_EXAMPLES Off)
set(GLFW_BUILD_TESTS Off)
set(GLFW_BUILD_DOCS Off)
add_subdirectory(ext/glfw)

add_library(SomeGraphicsEngine STATIC ${SRCS})

target_include_directories(SomeGraphicsEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(SomeGraphicsEngine PUBLIC glfw)
