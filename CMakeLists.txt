cmake_minimum_required(VERSION 3.9)
project(cpptok)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()

set(CPPTOK_PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})

##################################################################
###### C++17 clang (Travis CI)
##################################################################

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message("Building with clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

##################################################################
###### coverage build
##################################################################

if(ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

##################################################################
###### cpptok library
##################################################################

file(GLOB_RECURSE CPPTOK_LIBRARY_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE CPPTOK_LIBRARY_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/cpptok/*.h)

add_library(cpptok SHARED ${CPPTOK_LIBRARY_HDR_FILES} ${CPPTOK_LIBRARY_SRC_FILES})
target_compile_definitions(cpptok PRIVATE -DCPPTOK_BUILD_LIBRARY_SHARED)
target_include_directories(cpptok PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(cpptok PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(cpptok PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

##################################################################
###### apps & tests
##################################################################

#add_subdirectory(apps)
add_subdirectory(tests)
