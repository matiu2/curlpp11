#include "curl.hpp"
#include <bandit/bandit.h>
#include <sstream>

using namespace bandit;

go_bandit([]() {

  describe("curlpp11", [&]() {
    it("1.1. Can can get an HTTP page into a string", [&]() {
      std::string result;
      curl::Easy c;
      c.setOpt(CURLOPT_VERBOSE, 1);
      c.url("http://httpbin.org/").perform(result);
      AssertThat(
          result,
          Contains("Testing an HTTP Library can become difficult sometimes."));
    });
    it("1.2. Can be re-used", [&]() {
      curl::Easy c;
      std::string london;
      std::string beirut;
      c.url("http://api.openweathermap.org/data/2.5/find?q=London")
       .perform(london)
       .url("http://api.openweathermap.org/data/2.5/find?q=Beirut")
       .perform(beirut);
      AssertThat(london, Contains("London"));
      AssertThat(beirut, Contains("Beirut"));
    });
  });
});

int main(int argc, char** argv)
{
  curl::GlobalSentry curl;
  return bandit::run(argc, argv);
}

