cmake_minimum_required(VERSION 3.25)

project(SomeGraphics VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
  set(SOME_GRAPHICS_FLAGS
    "/W4"
    "/wd26439"
    "/external:anglebrackets"
    "/analyze:external-"
  )
else()
  set(SOME_GRAPHICS_FLAGS
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-Wshadow"
    "-Wconversion"
    $<$<CONFIG:Debug>:-g3>
  )
endif()

add_subdirectory("./engine")
add_subdirectory("./editor")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SomeGraphicsEditor)
