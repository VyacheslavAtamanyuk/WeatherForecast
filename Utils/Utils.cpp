#include "Utils.h"

DataConverter::DataConverter(WeatherDataAboutSomeCity& data): data(data) {}

ConvertedData DataConverter::ConvertFloatsToStreams(uint8_t day_idx) {
    std::ostringstream stream_for_max_temp;
    stream_for_max_temp << data.temperature_2m_max[day_idx];

    std::ostringstream stream_for_min_temp;
    stream_for_min_temp << data.temperature_2m_min[day_idx];

    std::ostringstream stream_for_rain_sum;
    stream_for_rain_sum << data.rain_sum[day_idx];

    std::ostringstream stream_for_wind_speed;
    stream_for_wind_speed << data.wind_speed_10m_max[day_idx];

    return {stream_for_max_temp.str(), stream_for_min_temp.str(), stream_for_wind_speed.str(), stream_for_rain_sum.str()};
}