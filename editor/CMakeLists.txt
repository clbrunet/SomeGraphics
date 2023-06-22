file(GLOB_RECURSE SRCS "src/*.cpp")

add_executable(SomeGraphicsEditor ${SRCS})

target_compile_options(SomeGraphicsEditor PRIVATE
  "-Wall"
  "-Wextra"
  "-Wpedantic"
  "-Wno-volatile"
  $<$<CONFIG:Debug>:-g3>
  )
target_include_directories(SomeGraphicsEditor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(SomeGraphicsEditor PUBLIC SomeGraphicsEngine)
