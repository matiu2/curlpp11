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
  # JSON needed for interpreting test results
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/json_class.hpp ${CMAKE_CURRENT_BINARY_DIR}/json_class.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parse_to_json_class.hpp ${CMAKE_CURRENT_BINARY_DIR}/parse_to_json_class.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/unicode.hpp ${CMAKE_CURRENT_BINARY_DIR}/unicode.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/utils.hpp ${CMAKE_CURRENT_BINARY_DIR}/utils.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/LocatingIterator.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/LocatingIterator.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/array.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/array.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/error.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/error.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/number.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/number.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/object.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/object.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/outer.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/outer.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/status.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/status.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/string.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/string.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/utf8_writer.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/utf8_writer.hpp)
  file(DOWNLOAD https://raw.githubusercontent.com/matiu2/jsonpp11/master/src/parser/utils.hpp ${CMAKE_CURRENT_BINARY_DIR}/parser/utils.hpp)
  set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif(${BUILD_TESTS})

add_subdirectory(src)
