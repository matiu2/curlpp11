#include "curl.hpp"

#include <istream>
#include <sstream>
#include <cassert>

namespace curl {

bool GlobalSentry::exists = false;

/// Called by curl when we have data to recv. Puts it in a stream.
extern "C" size_t onDataToRecv(char *ptr, size_t size, size_t nmemb,
                               void *userdata) {
  const size_t bytes = size * nmemb;
  std::ostream *response = static_cast<std::ostream *>(userdata);
  response->write(ptr, bytes);
  return bytes;
}

/// Called by curl when we have data to recv. Puts it in a string.
extern "C" size_t onDataToRecvString(char *ptr, size_t size, size_t nmemb,
                                     void *userdata) {
  const size_t bytes = size * nmemb;
  std::string* response = static_cast<std::string*>(userdata);
  response->reserve(response->size() + bytes);
  std::copy(ptr, ptr+bytes, std::back_inserter(*response));
  return bytes;
}

/// Called by curl when it wants data to send. Reads it from a stream.
extern "C" size_t onDataToSend(char *buffer, size_t size, size_t nitems,
                                void *instream) {
  const size_t bytes = size * nitems;
  auto data = static_cast<std::istream *>(instream);
  data->read(buffer, bytes);
  return data->gcount();
}

GlobalSentry::GlobalSentry() {
  assert(!exists); // There can be only one!
  exists = true;
  CURLcode result = curl_global_init(CURL_GLOBAL_SSL);
  if (result != 0) {
    std::stringstream msg;
    msg << "Couldn't Initialize Curl Library. Error code: " << result;
    throw std::runtime_error(std::move(msg.str()));
  }
}

GlobalSentry::~GlobalSentry() { curl_global_cleanup(); }

// Easy ///////////////////////////////

void Easy::checkError(CURLcode num) const {
  if (num != CURLE_OK)
    throw Err(num);
}

Easy& Easy::header(const char *header) {
  headers.add(header);
  setOpt(CURLOPT_HTTPHEADER, headers.base);
  return *this;
}

Easy& Easy::perform(std::ostream &result) {
  setOpt(CURLOPT_WRITEFUNCTION, onDataToRecv);
  setOpt(CURLOPT_WRITEDATA, &result);
  checkError(curl_easy_perform(handle));
  return *this;
}

Easy& Easy::perform(std::string &result) {
  setOpt(CURLOPT_WRITEFUNCTION, onDataToRecvString);
  setOpt(CURLOPT_WRITEDATA, &result);
  checkError(curl_easy_perform(handle));
  return *this;
}

Easy& Easy::url(const char* url) {
  setOpt(CURLOPT_URL, url);
  return *this;
};

Easy& Easy::customBody(std::ostream& result, size_t size) {
  setOpt(CURLOPT_READFUNCTION, onDataToSend);
  setOpt(CURLOPT_POSTFIELDSIZE, size);
  setOpt(CURLOPT_READDATA, &result);
  return *this;
}

Easy& Easy::customBody(std::ostream& result) {
  size_t pos = result.tellp();
  result.seekp(0, std::ios_base::end);
  size_t size = result.tellp();
  result.seekp(pos);
  return customBody(result, size);
}

Easy& Easy::userAgent(const char* agent) {
  setOpt(CURLOPT_USERAGENT, agent);
}

Easy& Easy::POST() {
  setOpt(CURLOPT_POST, 1);
}

Easy& Easy::PUT() {
  setOpt(CURLOPT_UPLOAD, 1);
}

Easy& Easy::GET() {
  setOpt(CURLOPT_HTTPGET, 1);
}

Easy& Easy::DELETE() {
  setOpt(CURLOPT_CUSTOMREQUEST, "DELETE");
}

}
