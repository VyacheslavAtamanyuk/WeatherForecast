#pragma once

#include "../API/API_Info.h"
#include "../ConfigInfo/ConfigInfo.h"
#include <cstdint>
#include <sstream>

struct ConvertedData {
    std::string max_temp;
    std::string min_temp;
    std::string wind_speed;
    std::string rain_sum;
};

class DataConverter {
public:
    DataConverter(WeatherDataAboutSomeCity& data);
    ConvertedData ConvertFloatsToStreams(uint8_t day_idx);
private:
    WeatherDataAboutSomeCity& data;
};