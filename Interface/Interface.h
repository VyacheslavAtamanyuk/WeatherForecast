#pragma once

#include "../ConfigInfo/ConfigInfo.h"
#include "../API/API_Info.h"
#include "../Utils/Utils.h"
#include "ftxui/dom/elements.hpp"

class Interface{
public:
    Interface(const ImmutableParameters& immutable_parameters, MutableParameters& mutable_parameters, APIInfo& weather_data);
    ftxui::Element DayForecast(DataConverter& converter, uint8_t day_idx);
    ftxui::Element MakeForecast();
    void OpenUI();
private:
    const ImmutableParameters& user_immutable_parameters;
    MutableParameters& user_mutable_parameters;
    APIInfo& weather_data;
};