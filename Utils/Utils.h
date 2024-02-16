#pragma once

#include <cstdint>
#include <sstream>
#include <nlohmann/json.hpp>

struct ConvertedDaysData {
    ConvertedDaysData(nlohmann::json& api_data);
    void ConvertFloatsToStreams(uint8_t day_idx);
    void clear();

    nlohmann::json& days_data;

    std::ostringstream stream_for_min_temp;
    std::ostringstream stream_for_max_temp;
    std::ostringstream stream_for_rain_sum;
    std::ostringstream stream_for_wind_speed;
};