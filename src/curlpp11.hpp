/// Miminmal curl wrapper
#pragma once

extern "C" {
#include <curl/curl.h>
}

#include <ostream>
#include <string>
#include <stdexcept>

namespace curl {

struct Err : std::runtime_error {
  CURLcode code;
  Err(CURLcode code) : std::runtime_error(curl_easy_strerror(code)), code(code) {}
};

struct CURLHeaders {
  curl_slist *base = nullptr;
  ~CURLHeaders() {
    if (base)
      curl_slist_free_all(base);
  }
  void add(const char *header) {
    curl_slist *last = curl_slist_append(base, header);
    if (!base)
      base = last;
  }
};

/// A create once per app, in the main curl sentry
class GlobalSentry {
private:
  static bool exists;
public:
  GlobalSentry();
  ~GlobalSentry();
};

class Easy {
protected:
  CURLHeaders headers;
  CURL* handle;
  void checkError(CURLcode num) const;
public:
  Easy() : handle(curl_easy_init()) {}
  ~Easy() { curl_easy_cleanup(handle); }
  /// Set CURL library options. See http://curl.haxx.se/libcurl/c/curl_easy_setopt.html
  template <typename ...T> Easy& setOpt(CURLoption opt, T... values) {
    checkError(curl_easy_setopt(handle, opt, values...));
    return *this;
  }
  template <typename... Types> Easy& getInfo(CURLINFO info, Types... args) {
    checkError(curl_easy_getinfo(handle, info, args...));
    return *this;
  }
  Easy& url(const std::string& url);
  Easy& header(const std::string& header);
  /// Perform the configured HTTP(S) request
  Easy& perform();
  /// Perform the configured HTTP(S) request, and give me the result in a stream
  Easy& perform(std::ostream& result);
  /// Perform the configured HTTP(S) request, and give me the result in a string
  Easy& perform(std::string& result);
  /// Set a custom body and size to send (useful for PUT and POST requests)
  Easy& customBody(std::istream& result, size_t size);
  /// Seeks to the end of the file to get the size, then goes back to the
  /// original position, then calls customBody(std::ostream& result, size_t
  /// size)
  Easy& customBody(std::istream& result);
  Easy& userAgent(const std::string& agent);
  /// We are issuing an HTTP POST request
  Easy& POST();
  /// We are issuing an HTTP PUT request
  Easy& PUT();
  /// We are issuing an HTTP GET request
  Easy& GET();
  /// We are issuing an HTTP DELETE request
  Easy& DELETE();
  /// Get the response code of the last perform() result
  long responseCode();
};

}
