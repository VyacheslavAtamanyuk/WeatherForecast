#include "Utils.h"

ConvertedDaysData::ConvertedDaysData(nlohmann::json& days_data): days_data(days_data) {}

void ConvertedDaysData::ConvertFloatsToStreams(uint8_t day_idx) {
    stream_for_max_temp << days_data["daily"]["temperature_2m_max"][day_idx];
    stream_for_min_temp << days_data["daily"]["temperature_2m_min"][day_idx];
    stream_for_rain_sum << days_data["daily"]["rain_sum"][day_idx];
    stream_for_wind_speed << days_data["daily"]["wind_speed_10m_max"][day_idx];
}

void ConvertedDaysData::clear() {
    stream_for_max_temp.str("");
    stream_for_min_temp.str("");
    stream_for_rain_sum.str("");
    stream_for_wind_speed.str("");
}