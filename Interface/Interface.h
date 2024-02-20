#pragma once

#include "../ConfigInfo/ConfigInfo.h"
#include "../API/API_Info.h"
#include "../Utils/Utils.h"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

class InterfacePresenter {
public:
    InterfacePresenter(const ImmutableParameters& immutable_parameters, MutableParameters& mutable_parameters, APIInfo& weather_data);
    ftxui::Element DayForecast(DataConverter& converter, uint8_t day_idx);
    ftxui::Element MakeForecast();

    const ImmutableParameters& GetImmutbaleParameters();
    MutableParameters& GetMutableParameters();
    APIInfo& GetWeatherData();
private:
    const ImmutableParameters& user_immutable_parameters;
    MutableParameters& user_mutable_parameters;
    APIInfo& weather_data;
};

class InterfaceController {
public:
    InterfaceController(InterfacePresenter& weather_display);
    void MakeLoop(ftxui::ScreenInteractive& screen);
    void OpenUI();
private:
    InterfacePresenter& weather_display;
};