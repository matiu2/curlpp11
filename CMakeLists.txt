cmake_minimum_required(VERSION 2.8)

project (curlpp11)

add_definitions(-fPIC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(BUILD_TESTS "Build the test suite and download the bandit testing framework" OFF)

if (${BUILD_TESTS})
  enable_testing()
  FIND_PACKAGE(CURL REQUIRED)
  FIND_PACKAGE(OpenSSL)
  find_path(BANDIT_INCLUDE_DIR bandit.hpp)
  if (${BANDIT_INCLUDE_DIR})
      SET(BANDIT_INCLUDE_DIR BANDIT_INCLUDE_DIR)
  else()
      include(ExternalProject)
      ExternalProject_Add(bandit
          PREFIX 3rd_party
          GIT_REPOSITORY https://github.com/joakimkarlsson/bandit.git
          TLS_VERIFY true
          TLS_CAINFO certs/DigiCertHighAssuranceEVRootCA.crt
          UPDATE_COMMAND "" # Skip annoying updates for every build
          INSTALL_COMMAND ""
      )
      SET(BANDIT_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/src/bandit)
  endif(${BANDIT_INCLUDE_DIR})

  # JSONPP11
  ExternalProject_Add(jsonpp11
      PREFIX 3rd_party
      GIT_REPOSITORY https://github.com/matiu2/jsonpp11.git
      TLS_VERIFY true
      TLS_CAINFO certs/DigiCertHighAssuranceEVRootCA.crt
      TEST_BEFORE_INSTALL 0
      TEST_COMMAND ctest
      UPDATE_COMMAND "" # Skip annoying updates for every build
      INSTALL_COMMAND ""
  )
  SET(JSON_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/src/jsonpp11/src)

endif(${BUILD_TESTS})

add_subdirectory(src)
