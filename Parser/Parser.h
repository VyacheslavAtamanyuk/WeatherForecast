#pragma once

#include <cstdint>
#include <vector>
#include <string>

struct ConfigParameters {
    bool UnpackConfig();
    bool IsCorrectAPI();
    static bool IsHaveInternetConnection();

    std::vector<std::string> cities;
    size_t frequency;
    std::string api_key;
    size_t city_idx;
    uint8_t forecast_days;
};