#include "API_Info.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <iostream>
#include <limits>

const uint8_t kMaxForecastDays = 16;
const float kNaN = std::numeric_limits<float>::quiet_NaN();

int APIInfo::GetInfoFromAPINinja(const ImmutableParameters& user_immutable_parameters, size_t city_idx) {
    cpr::Response request = cpr::Get(cpr::Url("https://api.api-ninjas.com/v1/city"),
                                     cpr::Parameters{{"name", user_immutable_parameters.GetCityOnSomeIdx(city_idx)}},
                                     cpr::Header{{"X-Api-Key", user_immutable_parameters.GetAPIKey()}});
    if (request.status_code != 200) {
        return 1;
    }
    std::string& required_string_for_json = request.text.erase(0,1); // remove extra square bracket
    required_string_for_json.pop_back(); // remove extra square bracket

    nlohmann::json api_ninja_info = nlohmann::json::parse(required_string_for_json);
    coordinates_cache.push_back({api_ninja_info["latitude"], api_ninja_info["longitude"]});
    return 0;
}

int APIInfo::GetInfoFromOpenMeteoAPI(float latitude, float longitude) {
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
    if (request.status_code != 200) {
        return 1;
    }
    nlohmann::json weather_data = nlohmann::json::parse(request.text);
    ConvertOpenMeteoData(weather_data);
    return 0;
}

void APIInfo::ConvertOpenMeteoData(nlohmann::json& weather_json) {
    WeatherDataAboutSomeCity weather_at_this_day;

    weather_at_this_day.forecast_days = weather_json["daily"]["time"];
    for (uint8_t idx = 0; idx < kMaxForecastDays; ++idx) {
        auto value_for_max_temp = weather_json["daily"]["temperature_2m_max"][idx];
        value_for_max_temp.is_null() ? (weather_at_this_day.temperature_2m_max.push_back(kNaN)) : weather_at_this_day.temperature_2m_max.push_back(value_for_max_temp);

        auto value_for_min_temp = weather_json["daily"]["temperature_2m_min"][idx];
        value_for_min_temp.is_null() ? (weather_at_this_day.temperature_2m_min.push_back(kNaN)) : weather_at_this_day.temperature_2m_min.push_back(value_for_min_temp);

        auto value_for_wind = weather_json["daily"]["wind_speed_10m_max"][idx];
        value_for_wind.is_null() ? (weather_at_this_day.wind_speed_10m_max.push_back(kNaN)) : weather_at_this_day.wind_speed_10m_max.push_back(value_for_wind);

        auto value_for_rain = weather_json["daily"]["rain_sum"][idx];
        value_for_rain.is_null() ? (weather_at_this_day.rain_sum.push_back(kNaN)) : weather_at_this_day.rain_sum.push_back(value_for_rain);
    }
    weather_cache.push_back(weather_at_this_day);
}


int APIInfo::GetAllForecasts(const ImmutableParameters& user_immutable_parameters, bool is_first_iteration) {
    bool something_wrong = false;
    std::vector<WeatherDataAboutSomeCity> reserve_copy = weather_cache;
    weather_cache.clear();

    for (size_t idx = 0; idx < user_immutable_parameters.GetCities().size(); ++idx) {
        if (!IsHaveInternetConnection()) {
            something_wrong = true;
            break;
        }

        if (coordinates_cache.size() < user_immutable_parameters.GetCities().size()) {
            if (GetInfoFromAPINinja(user_immutable_parameters, idx) == 1) {
                something_wrong = true;
                break;
            }
        }

        if (GetInfoFromOpenMeteoAPI(coordinates_cache[idx].latitude, coordinates_cache[idx].longitude) == 1) {
            something_wrong = true;
            break;
        }
    }

    if (something_wrong && !is_first_iteration) {
        weather_cache = reserve_copy;
        return 0;
    } else if (something_wrong && is_first_iteration) {
        std::cerr << "Something went wrong";
        return 1;
    } else if (!something_wrong && is_first_iteration) {
        return 0;
    } else {
        return 0;
    }
}

std::vector<WeatherDataAboutSomeCity>& APIInfo::GetWeatherCache() {
    return weather_cache;
}

bool APIInfo::IsHaveInternetConnection() {
    cpr::Response r = cpr::Get(cpr::Url("www.google.com"));
    if (r.text.empty()) {
        return false;
    }
    return true;
}