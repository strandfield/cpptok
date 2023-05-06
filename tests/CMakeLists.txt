
enable_testing()

if(NOT DEFINED CACHE{BUILD_CPPTOK_TESTS})
  set(BUILD_CPPTOK_TESTS ON CACHE BOOL "whether to build cpptok test")
endif()

if(BUILD_CPPTOK_TESTS)

  if(NOT DEFINED CPPTOK_CATCH2_SINGLE_HEADER_FILE OR NOT CPPTOK_CATCH2_SINGLE_HEADER_FILE)

    message("Downloading Catch2")
    file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp "${CMAKE_CURRENT_LIST_DIR}/catch.hpp" SHOW_PROGRESS)
  
    set(CPPTOK_CATCH2_SINGLE_HEADER_FILE "${CMAKE_CURRENT_LIST_DIR}/catch.hpp" CACHE STRING "Path of catch.hpp")

  endif()

  add_executable(TEST_cpptok "tests.cpp" ${CPPTOK_CATCH2_SINGLE_HEADER_FILE})
  target_link_libraries(TEST_cpptok cpptok)

  set_target_properties(TEST_cpptok PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(TEST_cpptok PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

  add_test(NAME TEST_cpptok COMMAND TEST_cpptok)

endif()
