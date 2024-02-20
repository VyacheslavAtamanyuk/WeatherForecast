#pragma once

#include "../ConfigInfo/ConfigInfo.h"
#include "nlohmann/json.hpp"

struct WeatherDataAboutSomeCity {
    std::vector<std::string> forecast_days;
    std::vector<float> temperature_2m_max;
    std::vector<float> temperature_2m_min;
    std::vector<float> wind_speed_10m_max;
    std::vector<float> rain_sum;
};

struct LocationDataAboutSomeCity {
    float latitude;
    float longitude;
};

class APIInfo {
public:
    int GetInfoFromAPINinja(const ImmutableParameters& user_immutable_parameters, size_t city_idx);
    int GetInfoFromOpenMeteoAPI(float latitude, float longitude);
    void ConvertOpenMeteoData(nlohmann::json& weather_json);
    int GetAllForecasts(const ImmutableParameters& user_immutable_parameters, bool is_first_iteration);
    bool IsHaveInternetConnection();
    std::vector<WeatherDataAboutSomeCity>& GetWeatherCache();
private:
    std::vector<WeatherDataAboutSomeCity> weather_cache;
    std::vector<LocationDataAboutSomeCity> coordinates_cache;
};
