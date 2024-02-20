#include "Interface.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include <chrono>

const uint8_t kDaysInOneRow = 4;
const uint8_t kSufficientWidth = 35;
const char next = 'n';
const char previous = 'p';
const char more_days = '+';
const char less_days = '-';
const bool is_it_first_iteration = false;

InterfacePresenter::InterfacePresenter(const ImmutableParameters& immutable_parameters, MutableParameters& mutable_parameters, APIInfo& weather_data): user_immutable_parameters(immutable_parameters), user_mutable_parameters(mutable_parameters), weather_data(weather_data)  {}

ftxui::Element InterfacePresenter::DayForecast(DataConverter& converter, uint8_t day_idx) {
    using namespace ftxui;

    ConvertedData converted_weather_data = converter.ConvertFloatsToStreams(day_idx);
    return vbox(text(weather_data.GetWeatherCache()[user_mutable_parameters.GetCityIdx()].forecast_days[day_idx]), separator(),
                text("temperature_2m_max: " + converted_weather_data.max_temp + " °C"),
                text("temperature_2m_min: " + converted_weather_data.min_temp + " °C"),
                text("rain_sum: " + converted_weather_data.rain_sum + " mm"),
                text("wind_speed_10m_max: " + converted_weather_data.wind_speed + " km/h")) | bold | border | size(WIDTH, EQUAL, kSufficientWidth);
}

ftxui::Element InterfacePresenter::MakeForecast() {
    using namespace ftxui;

    Elements rows;
    rows.push_back(hbox(filler(), hbox(text("Your city: "), text(user_immutable_parameters.GetCityOnSomeIdx(user_mutable_parameters.GetCityIdx())) | color(Color::Orange1), separator(),
                                       text("Forecast days: "), text(std::to_string(user_mutable_parameters.GetForecastDays())) | color(Color::Orange1)) | bold | border, filler()));

    DataConverter converter(weather_data.GetWeatherCache()[user_mutable_parameters.GetCityIdx()]);

    int day_idx = 0;
    Elements row;

    while (day_idx < (user_mutable_parameters.GetForecastDays() / kDaysInOneRow) * kDaysInOneRow) {
        row.push_back(DayForecast(converter, day_idx));

        if ((day_idx + 1) % kDaysInOneRow == 0) {
            rows.push_back(hbox(filler(),hbox(row), filler()));
            row.clear();
        }
        ++day_idx;
    }

    while (day_idx < user_mutable_parameters.GetForecastDays()) {
        row.push_back(DayForecast(converter, day_idx));
        ++day_idx;
    }
    rows.push_back(hbox(filler(), hbox(row), filler()));

    return vbox(rows);
}

const ImmutableParameters& InterfacePresenter::GetImmutbaleParameters() {
    return user_immutable_parameters;
}

MutableParameters& InterfacePresenter::GetMutableParameters() {
    return user_mutable_parameters;
}

APIInfo& InterfacePresenter::GetWeatherData() {
    return weather_data;
}


InterfaceController::InterfaceController(InterfacePresenter& weather_display): weather_display(weather_display) {}

void InterfaceController::MakeLoop(ftxui::ScreenInteractive& screen) {
    using namespace ftxui;

    Component renderer = Renderer([&] {
        return weather_display.MakeForecast();
    });

    std::chrono::time_point start = std::chrono::system_clock::now();
    Component component = CatchEvent(renderer, [&](Event event) {
        std::chrono::time_point current = std::chrono::system_clock::now();
        if (current - start >= std::chrono::seconds(weather_display.GetImmutbaleParameters().GetFrequency())) {
            weather_display.GetWeatherData().GetAllForecasts(weather_display.GetImmutbaleParameters(), is_it_first_iteration);
            start = std::chrono::system_clock::now();
            return true;
        }
        if (event == Event::Escape) {
            screen.Exit();
        }
        if (event == Event::Character(more_days)) {
            weather_display.GetMutableParameters().IncreaseForecastDays();
            return true;
        }
        if (event == Event::Character(less_days)) {
            weather_display.GetMutableParameters().DecreaseForecastDays();
            return true;
        }
        if (event == Event::Character(next)) {
            weather_display.GetMutableParameters().IncreaseCityIdx(weather_display.GetImmutbaleParameters());
            return true;
        }
        if (event == Event::Character(previous)) {
            weather_display.GetMutableParameters().DecreaseCityIdx();
            return true;
        }
        return false;
    });

    screen.Loop(component);
}

void InterfaceController::OpenUI() {
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::FullscreenAlternateScreen();
    screen.SetCursor({0,0});

    MakeLoop(screen);
}
