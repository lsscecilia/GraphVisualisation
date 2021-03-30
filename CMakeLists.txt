cmake_minimum_required(VERSION 3.9)

#set project name
project(graph_visualisation VERSION "1.0")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE src/algorithm.cpp src/main.cpp)
set(CONFIGURE_FILE src/config.h.in)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#set executable
add_executable(${PROJECT_NAME} ${SOURCE}) 

configure_file( src/config.h.in ${CMAKE_BINARY_DIR}/bin/config.h)
include_directories( ${CMAKE_BINARY_DIR}/bin/ ) # Make sure it can be included...

add_library(
  algorithm src/algorithm.cpp
)

target_link_libraries(
  graph_visualisation
  algorithm
)

#google test
include(FetchContent)

option(graph_visualisation_build_tests "Build unit tests" ON)

if (graph_visualisation_build_tests)
  find_package(GTest 1.10.0 QUIET)
  if (NOT GTest_FOUND)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG release-1.10.0)

    FetchContent_GetProperties(googletest)
    if (NOT googletest_POPULATED)
      FetchContent_Populate(googletest)
      add_subdirectory(
        ${googletest_SOURCE_DIR}
        ${googletest_BINARY_DIR}
        EXCLUDE_FROM_ALL)
      add_library(GTest::Main ALIAS gtest_main)
    endif ()
  endif ()
endif ()

if (graph_visualisation_build_tests)
  add_executable(graph_visualisation_test test/graph_visualisation_test.cpp)
  target_link_libraries(graph_visualisation_test
    algorithm
    GTest::Main)
endif ()
