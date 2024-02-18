#include "API_Info.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <sstream>

const uint8_t kMaxForecastDays = 16;

void APIInfo::GetInfoFromAPINinja(const ImmutableParameters& user_immutable_parameters, size_t city_idx) {
    cpr::Response request = cpr::Get(cpr::Url("https://api.api-ninjas.com/v1/city"),
                                     cpr::Parameters{{"name", user_immutable_parameters.GetCityOnSomeIdx(city_idx)}},
                                     cpr::Header{{"X-Api-Key", user_immutable_parameters.GetAPIKey()}});
    std::string& required_string_for_json = request.text.erase(0,1); // remove extra square bracket
    required_string_for_json.pop_back(); // remove extra square bracket

    nlohmann::json api_ninja_info = nlohmann::json::parse(required_string_for_json);
    coordinates_cache.push_back({api_ninja_info["latitude"], api_ninja_info["longitude"]});
}

void APIInfo::GetInfoFromOpenMeteoAPI(float latitude, float longitude) {
    std::ostringstream stream_for_latitude;
    stream_for_latitude << latitude;
    std::string string_for_latitude = stream_for_latitude.str();

    std::ostringstream stream_for_longitude;
    stream_for_longitude << longitude;
    std::string string_for_longitude = stream_for_longitude.str();

    std::string string_for_forecast_days = std::to_string(kMaxForecastDays);

    cpr::Response request = cpr::Get(cpr::Url("https://api.open-meteo.com/v1/forecast"),
                                      cpr::Parameters{{"latitude", string_for_latitude},
                                                      {"longitude", string_for_longitude},
                                                      {"forecast_days", string_for_forecast_days},
                                                      {"daily", "temperature_2m_min"},
                                                      {"daily", "temperature_2m_max"},
                                                      {"daily", "rain_sum"},
                                                      {"daily", "wind_speed_10m_max"}});
    nlohmann::json data = nlohmann::json::parse(request.text);
    weather_cache.push_back({data["daily"]["time"], data["daily"]["temperature_2m_max"], data["daily"]["temperature_2m_min"], data["daily"]["wind_speed_10m_max"], data["daily"]["rain_sum"]});
}

void APIInfo::GetAllForecasts(const ImmutableParameters& user_immutable_parameters) {
    weather_cache.clear();
    for (size_t idx = 0; idx < user_immutable_parameters.GetCities().size(); ++idx) {
        if (coordinates_cache.size() < user_immutable_parameters.GetCities().size()) {
            GetInfoFromAPINinja(user_immutable_parameters, idx);
        }
        GetInfoFromOpenMeteoAPI(coordinates_cache[idx].latitude, coordinates_cache[idx].longitude);
    }
}

std::vector<WeatherDataAboutSomeCity>& APIInfo::GetWeatherCache() {
    return weather_cache;
}