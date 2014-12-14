curlpp11
========

A super easy c++11 curl wrapper.

It only wraps the bits that I need; I'm not planning to any more development on it unless I need it, but all contributions and requests are welcome.

## Usage

Just download the two files into your project:

 * https://raw.githubusercontent.com/matiu2/curlpp11/master/src/curl.hpp
 * https://raw.githubusercontent.com/matiu2/curlpp11/master/src/curl.cpp

### Usage Notes

 * Always use the global sentry in your main function (see below)
 * One curl::Easy per thread

### Always use the global sentry in your main function

    #include "curl.hpp"

    int main(int argc, char** argv) {
      curl::GlobalSentry curl;
      ...
    }

## Example Usage

### Configure then perform and re-use
    
    curl::Easy c;
    std::string london;
    std::string beirut;
    c.url("http://api.openweathermap.org/data/2.5/find?q=London")
      .perform(london)
      .url("http://api.openweathermap.org/data/2.5/find?q=Beirut")
      .perform(beirut);

### POST and PUT and DELETE

    curl::Easy c;
    c.url("http://httpbin.org/delete")
     .DELETE()
     .perform();

### Remember to check the response codes (because we don't know if 500 is what you were hoping for)

    c.perform();
    if (c.responseCode() != 200)
      blowUpTheWorld();

