#include "API_Info.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <iostream>

const uint8_t kMaxForecastDays = 16;

API::API(const std::string& api_key): api_key(api_key) {}

std::pair<float, float> API::GetInfoFromAPINinja(const std::string& city) {
    cpr::Response request = cpr::Get(cpr::Url("https://api.api-ninjas.com/v1/city"),
                                     cpr::Parameters{{"name", city}},
                                     cpr::Header{{"X-Api-Key", api_key}});
    std::string& required_string_for_json = request.text.erase(0,1); // remove extra square bracket
    required_string_for_json.pop_back(); // remove extra square bracker

    nlohmann::json api_ninja_info = nlohmann::json::parse(required_string_for_json);
    return {api_ninja_info["latitude"], api_ninja_info["longitude"]};
}

void API::GetInfoFromOpenMeteoAPI(float latitude, float longitude) {
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
    weather_cache.push_back(nlohmann::json::parse(request.text));
}

void API::GetAllForecasts(std::vector<std::string>& cities) {
    for (size_t i = 0; i < cities.size(); ++i) {
        if (coordinates_cache.size() < cities.size()) {
            std::pair<float, float> coordinates = GetInfoFromAPINinja(cities[i]);
            coordinates_cache.push_back(coordinates);
            GetInfoFromOpenMeteoAPI(coordinates.first, coordinates.second);
        } else {
            GetInfoFromOpenMeteoAPI(coordinates_cache[i].first, coordinates_cache[i].second);
        }
    }
}