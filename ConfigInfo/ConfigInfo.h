#pragma once

#include <cstdint>
#include <vector>
#include <string>

struct ConfigInfo {
    bool UnpackInfoFromUserJson();
    void IncreaseCityIdx();
    void DecreaseCityIdx();
    void IncreaseForecastDays();
    void DecreaseForecastDays();

    std::vector<std::string> cities;
    size_t city_idx = 0;
    uint8_t forecast_days;
    size_t frequency;
    std::string api_key;
};