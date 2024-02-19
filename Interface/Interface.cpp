#include "Interface.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include <chrono>

const uint8_t kDaysInOneRow = 4;
const uint8_t kSufficientWidth = 35;
const char next = 'n';
const char previous = 'p';
const char more_days = '+';
const char less_days = '-';

Interface::Interface(const ImmutableParameters& immutable_parameters, MutableParameters& mutable_parameters, APIInfo& weather_data): user_immutable_parameters(immutable_parameters), user_mutable_parameters(mutable_parameters), weather_data(weather_data) {}

ftxui::Element Interface::DayForecast(DataConverter& converter, uint8_t day_idx) {
    using namespace ftxui;

    ConvertedData converted_weather_data = converter.ConvertFloatsToStreams(day_idx);
    return vbox(text(weather_data.GetWeatherCache()[user_mutable_parameters.GetCityIdx()].forecast_days[day_idx]), separator(),
                text("temperature_2m_max: " + converted_weather_data.max_temp + " °C"),
                text("temperature_2m_min: " + converted_weather_data.min_temp + " °C"),
                text("rain_sum: " + converted_weather_data.rain_sum + " mm"),
                text("wind_speed_10m_max: " + converted_weather_data.wind_speed + " km/h")) | bold | border | size(WIDTH, EQUAL, kSufficientWidth);
}

ftxui::Element Interface::MakeForecast() {
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

void Interface::OpenUI() {
    using namespace ftxui;

    auto screen = ScreenInteractive::FullscreenAlternateScreen();
    screen.SetCursor({0,0});

    auto renderer = Renderer([&] {
        return MakeForecast();
    });

    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Escape) {
            screen.Exit();
        }
        if (event == Event::Character(more_days)) {
            user_mutable_parameters.IncreaseForecastDays();
        }
        if (event == Event::Character(less_days)) {
            user_mutable_parameters.DecreaseForecastDays();
        }
        if (event == Event::Character(next)) {
            user_mutable_parameters.IncreaseCityIdx(user_immutable_parameters);
        }
        if (event == Event::Character(previous)) {
            user_mutable_parameters.DecreaseCityIdx();
        }
        return true;
    });

    auto start = std::chrono::system_clock::now();
    Loop loop(&screen, component);
    while (!loop.HasQuitted()) {
        auto current = std::chrono::system_clock::now();
        if (current - start > std::chrono::seconds(user_immutable_parameters.GetFrequency())) {
            start = std::chrono::system_clock::now();
            weather_data.GetAllForecasts(user_immutable_parameters);
            screen.PostEvent(Event::Custom); // event also will be sent on line 97, but I do it here for faster rendering
        }
        loop.RunOnce();
    }
}
