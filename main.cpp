#include <cpprest/uri.h>
#include <cpprest/http_client.h>
#include <cpprest/uri_builder.h>
#include <cpprest/json.h>
#include <pplx/pplxtasks.h>

int main(int argc, const char *const argv[]) {
  using namespace web;
  using namespace web::http;
  using namespace web::http::client;
  using namespace web::json;

  using namespace utility; // for string_t
  using namespace pplx;    // for asynchronous tasks

  if (argc < 2) {
    std::cout << "Usage: weather-exe city\n";
    return 1;
  }

  const string_t city = argv[1];
  const string_t base_url = "https://api.openweathermap.org";
  const string_t api_key = "2fe721c7b3973189d660c79c492c06d6";

  uri uri_(base_url);

  value json_val;
  http_client client(uri_);
  uri_builder builder;

  client
      .request(methods::GET,
               uri_builder("/data/2.5/weather")
                   .set_query("q=" + city, /* do encoding */ true)
                   .append_query("appid", api_key)
                   .append_query("units", "metric")
                   .to_string(),
               json_val)
      .then([](const task<http_response> &response) -> task<json::value> {
        try {
          return response.get().extract_json();
        } catch (http_exception &e) {
          std::cerr << e.what() << ' ' << e.error_code() << '\n';
        }
      })
      .then([](const task<json::value> &val) -> void {
        auto printWeather = [](const value &val) {
          const string_t city = val.at("name").as_string();
          const string_t state = val.at("weather").as_array().at(0).at("description").as_string();
          const int temperature = val.at("main").at("temp").as_integer();
          std::cerr << state << ' ' << temperature << ' ' << city << '\n';
        };

        try {
          printWeather(val.get());
        } catch (json_exception &e) {
          std::cout << "City name not found\n"; // e.what();
        }
      })
      .wait();

  return 0;
}

