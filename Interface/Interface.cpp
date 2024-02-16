#include "Interface.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include <chrono>

const uint8_t kDaysInOneRow = 4;
const uint8_t kSufficientWidth = 35;

Interface::Interface(ConfigInfo& config_data, API& api_data): config_data(config_data), api_data(api_data) {}

ftxui::Element Interface::DayForecast(ConvertedDaysData& converted_info, uint8_t day_idx) {
    using namespace ftxui;

    converted_info.ConvertFloatsToStreams(day_idx);
    return vbox(text(std::string(api_data.weather_cache[config_data.city_idx]["daily"]["time"][day_idx])), separator(),
                text("temperature_2m_max: " + converted_info.stream_for_max_temp.str() + " °C"),
                text("temperature_2m_min: " + converted_info.stream_for_min_temp.str() + " °C"),
                text("rain_sum: " + converted_info.stream_for_rain_sum.str() + " mm"),
                text("wind_speed_10m_max: " + converted_info.stream_for_wind_speed.str() + " km/h")) | bold | border | size(WIDTH, EQUAL, kSufficientWidth);
}

ftxui::Element Interface::MakeForecast() {
    using namespace ftxui;
    Elements rows;
    rows.push_back(hbox(filler(), hbox(text("Your city: "), text(config_data.cities[config_data.city_idx]) | color(Color::Orange1), separator(),
                                       text("Forecast days: "), text(std::to_string(config_data.forecast_days)) | color(Color::Orange1)) | bold | border, filler()));

    ConvertedDaysData converted_days_info(api_data.weather_cache[config_data.city_idx]);

    int day_idx = 0;
    Elements row;

    while (day_idx < (config_data.forecast_days / kDaysInOneRow) * kDaysInOneRow) {
        row.push_back(DayForecast(converted_days_info, day_idx));
        converted_days_info.clear();

        if ((day_idx + 1) % kDaysInOneRow == 0) {
            rows.push_back(hbox(filler(),hbox(row), filler()));
            row.clear();
        }
        ++day_idx;
    }

    while (day_idx < config_data.forecast_days) {
        row.push_back(DayForecast(converted_days_info, day_idx));
        converted_days_info.clear();
        ++day_idx;
    }
    rows.push_back(hbox(filler(), hbox(row), filler()));

    return vbox(std::move(rows));
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
        if (event == Event::Character('+')) {
            config_data.IncreaseForecastDays();
        }
        if (event == Event::Character(('-'))) {
            config_data.DecreaseForecastDays();
        }
        if (event == Event::Character('n')) {
            config_data.IncreaseCityIdx();
        }
        if (event == Event::Character('p')) {
            config_data.DecreaseCityIdx();
        }
        return true;
    });

    auto start = std::chrono::system_clock::now();
    Loop loop(&screen, component);
    while (!loop.HasQuitted()) {
        auto current = std::chrono::system_clock::now();
        if (current - start > std::chrono::seconds(config_data.frequency)) {
            start = std::chrono::system_clock::now();
            api_data.GetAllForecasts(config_data.cities);
            screen.PostEvent(Event::Custom); // event also will be sent on line 110, but I do it here for faster rendering
        }
        loop.RunOnce();
    }
}
