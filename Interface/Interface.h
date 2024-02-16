#pragma once

#include "../ConfigInfo/ConfigInfo.h"
#include "../API/API_Info.h"
#include "../Utils/Utils.h"
#include "ftxui/dom/elements.hpp"

class Interface{
public:
    Interface(ConfigInfo& config_data, API& api_data);
    ftxui::Element DayForecast(ConvertedDaysData& converted_info, uint8_t day_idx);
    ftxui::Element MakeForecast();
    void OpenUI();
private:
    ConfigInfo& config_data;
    API& api_data;
};