project(src)

message("Looking at source")

add_library(curlpp11 curlpp11.cpp)
target_link_libraries(curlpp11 ${CURL_LIBRARIES})
set_target_properties(curlpp11 PROPERTIES PUBLIC_HEADER curlpp11.hpp)

install(TARGETS curlpp11
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

if (${BUILD_TESTS})
  message("Building tests")
  add_definitions(-std=c++11)
  add_executable(tests test.cpp)
  target_link_libraries(tests curlpp11)
  add_test(tests test)
  if (${OPENSSL_FOUND})
    set_property(
      TARGET tests
      APPEND
      PROPERTY INCLUDE_DIRECTORIES ${JSON_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${BANDIT_INCLUDE_DIR})
    target_link_libraries(tests ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})
  else()
    set_property(
      TARGET tests
      APPEND
      PROPERTY INCLUDE_DIRECTORIES ${JSON_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${BANDIT_INCLUDE_DIR})
    target_link_libraries(tests ${CURL_LIBRARIES})
  endif()
endif()
