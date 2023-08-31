file(GLOB_RECURSE SRCS "src/*.cpp")

add_executable(SomeGraphicsEditor ${SRCS})

target_compile_options(SomeGraphicsEditor PRIVATE ${SOME_GRAPHICS_FLAGS})
target_include_directories(SomeGraphicsEditor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(SomeGraphicsEditor PUBLIC SomeGraphicsEngine)

set_target_properties(SomeGraphicsEditor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
