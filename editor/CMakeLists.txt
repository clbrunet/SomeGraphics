file(GLOB_RECURSE SRCS "src/*.cpp")

add_executable(SomeGraphicsEditor ${SRCS})

if(CMAKE_HOST_WIN32)
  target_compile_options(SomeGraphicsEditor PRIVATE "-Wall")
else()
  target_compile_options(SomeGraphicsEditor PRIVATE
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    $<$<CONFIG:Debug>:-g3>
    )
endif()
target_include_directories(SomeGraphicsEditor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(SomeGraphicsEditor PUBLIC SomeGraphicsEngine)
