#include "curlpp11.hpp"
#include <bandit/bandit.h>
#include <sstream>
#include "json_class.hpp"
#include "parse_to_json_class.hpp"

using namespace bandit;

go_bandit([]() {

  describe("curlpp11", [&]() {
    it("1.1. Can can get an HTTP page into a string", [&]() {
      std::string result;
      curl::Easy c;
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
    it("1.3. Can do a delete", [&]() {
      curl::Easy c;
      c.url("http://httpbin.org/delete").DELETE().perform();
      AssertThat(c.responseCode(), Equals(200));
      c.url("http://httpbin.org").DELETE().perform();
      AssertThat(c.responseCode(), Equals(405));
    });
    it("1.4. Can post a stream", [&]() {
      std::stringstream data;
      data << "This is some data";
      std::string reply;
      curl::Easy c;
      c.url("http://httpbin.org/post").customBody(data, data.str().size()).POST().perform(reply);
      std::cout << "=============" << std::endl << reply << "=============" << std::endl; 
      std::cout.flush();
      json::JSON j = json::readValue(reply.begin(),  reply.end());
      json::JMap& form = j["form"];
      for (auto p : form)
        std::cout << p.first << " = " << p.second << std::endl;
      AssertThat(static_cast<const std::string&>(j["form"]["This is some data"]), Equals(""));
    });
  });
});

int main(int argc, char** argv)
{
  curl::GlobalSentry curl;
  return bandit::run(argc, argv);
}

