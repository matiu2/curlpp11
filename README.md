curlpp11
========

A super easy c++11 curl wrapper.

## Usage

 * Always use the global sentry in your main func
 * One curl::Easy per thread

### Always use the global sentry in your main func

    #include "curl.hpp"

    int main(int argc, char** argv) {
      curl::GlobalSentry curl;
      ...
    }

### Configure then perform and re-use
    
      curl::Easy c;
      std::string london;
      std::string beirut;
      c.url("http://api.openweathermap.org/data/2.5/find?q=London")
       .perform(london)
       .url("http://api.openweathermap.org/data/2.5/find?q=Beirut")
       .perform(beirut);

### POST and PUT and DELETE


