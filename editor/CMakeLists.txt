file(GLOB_RECURSE SRCS "src/*.cpp")

add_executable(SomeGraphicsEditor ${SRCS})

if(MSVC)
  target_compile_options(SomeGraphicsEditor PRIVATE "/W4" "/wd26439")
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

set_target_properties(SomeGraphicsEditor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
