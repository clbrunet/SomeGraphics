file(GLOB SRCS "imgui/*.cpp")
list(APPEND SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
  )

add_library(imgui STATIC ${SRCS})

target_include_directories(imgui PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends"
  )
target_link_libraries(imgui PUBLIC glfw)
