#include "curlpp11.hpp"
#include <bandit/bandit.h>
#include <sstream>
#include "json_class.hpp"
#include "parse_to_json_class.hpp"

using namespace bandit;
using namespace snowhouse;

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
      c.url("https://en.wikipedia.org/wiki/London")
          .perform(london)
          .url("https://en.wikipedia.org/wiki/Beirut")
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
      c.url("http://httpbin.org/post")
          .customBody(data, data.str().size())
          .POST()
          .perform(reply);
      std::cout << "=============" << std::endl << reply
                << "=============" << std::endl;
      std::cout.flush();
      json::JSON j = json::readValue(reply.begin(), reply.end());
      json::JMap &form = j["form"];
      for (auto p : form)
        std::cout << p.first << " = " << p.second << std::endl;
      AssertThat(
          static_cast<const std::string &>(j["form"]["This is some data"]),
          Equals(""));
    });
    it("1.5. Can URL encode", [&]() {
      curl::Easy c;
      std::string raw = "&[{}hello";
      std::string expected = "%26%5B%7B%7Dhello";
      std::string result = c.urlEncode(raw);
      AssertThat(result, Equals(expected));
    });
    it("1.6. Can URL encode params", [&]() {
      curl::Easy c;
      std::vector<std::pair<std::string, std::string>> params = {
          {"dog", "rover"}, {"&[{}hello", "_-]+)*=}there"}};
      std::string expected =
          "dog=rover&%26%5B%7B%7Dhello=_-%5D%2B%29%2A%3D%7Dthere";
      std::string result = c.urlEncodeParams(params.cbegin(), params.cend());
      AssertThat(result, Equals(expected));
    });
    it("1.7. Keeps headers", [&]() {
      curl::Easy curl;
      std::string as;
      std::string bs;
      std::string cs;
      curl.url("http://httpbin.org/headers")
          .header("Header-A: 1")
          .perform(as)
          .reset() // This will remove all curl options. The header list exists
                   // still, but is not associated with the curl handle any more
          .url("http://httpbin.org/headers")
          .perform(bs) // Because we didn't call 'header' again, the old header
                       // will not be present
          .reset()
          .url("http://httpbin.org/headers")
          .header() // Now we called header, it will re-associate the old hear
                    // lists with the curl handle
          .perform(cs);
      using namespace json;
      JMap a = readValue(as.begin(), as.end()).at("headers");
      JMap b = readValue(bs.begin(), bs.end()).at("headers");
      JMap c = readValue(cs.begin(), cs.end()).at("headers");
      AssertThat(a, Contains("Header-A"));
      AssertThat(b, Is().Not().Containing("Header-A"));
      AssertThat(c, Contains("Header-A"));
    });

  });
});

int main(int argc, char** argv)
{
  curl::GlobalSentry curl;
  return bandit::run(argc, argv);
}

