cmake_minimum_required(VERSION 3.0.0)
project(contextcpp VERSION 0.1.0)

option(CONTEXT_WITH_EXAMPLES "Build the context examples")

add_library(contextcpp INTERFACE)

target_include_directories(contextcpp INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:include>)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


if (CONTEXT_WITH_EXAMPLES)
  add_subdirectory(examples)
endif()

if (CONTEXT_WITH_TESTS)
  include(CTest)
  enable_testing()

  add_subdirectory(test)
endif()