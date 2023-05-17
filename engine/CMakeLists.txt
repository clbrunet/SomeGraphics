file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(SomeGraphicsEngine STATIC ${SRCS})

target_include_directories(SomeGraphicsEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
