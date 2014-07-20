project(src)

message("Looking at source")

if (${BUILD_TESTS})
  message("Building tests")
  add_executable(tests test.cpp curl.cpp)
  add_test(tests test)
  if (${OPENSSL_FOUND})
    set_property(
      TARGET tests
      APPEND
      PROPERTY INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${BANDIT_INCLUDE_DIR})
    target_link_libraries(tests ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})
  else()
    set_property(
      TARGET tests
      APPEND
      PROPERTY INCLUDE_DIRECTORIES ${CURL_INCLUDE_DIR} ${BANDIT_INCLUDE_DIR})
    target_link_libraries(tests ${CURL_LIBRARIES})
  endif()
endif()
