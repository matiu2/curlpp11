cmake_minimum_required(VERSION 2.8)

project (curlpp11)

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
endif(${BUILD_TESTS})

add_subdirectory(src)
