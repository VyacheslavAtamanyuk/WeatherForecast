#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <string_view>

struct API {
    API(const std::string& api_key);
    std::pair<float, float> GetInfoFromAPINinja(const std::string& city);
    void GetInfoFromOpenMeteoAPI(float latitude, float longitude);
    void GetAllForecasts(std::vector<std::string>& cities);

    const std::string& api_key;
    std::vector<nlohmann::json> weather_cache;
    std::vector<std::pair<float, float>> coordinates_cache;
};