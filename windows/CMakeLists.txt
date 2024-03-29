cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "flutter_native_view")
project(${PROJECT_NAME} LANGUAGES CXX)

set(PLUGIN_NAME "flutter_native_view_plugin")

add_library(${PLUGIN_NAME} SHARED
  "../core/utils.cc"
  "../core/ffi_api.cc"
  "../core/native_view_core.cc"
  "../core/native_view_container.cc"
  "../core/native_view_subclass_proc.cc"
  "flutter_native_view_plugin.cc"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../core"
)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/../core"
)

target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)

set(flutter_native_view_bundled_libraries
  ""
  PARENT_SCOPE
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4302 /wd4311 /wd4312")
