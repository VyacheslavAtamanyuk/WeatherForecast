#include "ConfigInfo.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

MutableParameters::MutableParameters(size_t city_idx, uint8_t forecast_days): city_idx(city_idx), forecast_days(forecast_days) {}

void MutableParameters::IncreaseForecastDays() {
    if (forecast_days < 16) {
        ++forecast_days;
    }
}

void MutableParameters::DecreaseForecastDays() {
    if (forecast_days > 0) {
        --forecast_days;
    }
}

void MutableParameters::IncreaseCityIdx(const ImmutableParameters& user_immutable_parameters) {
    if (city_idx + 1 < user_immutable_parameters.GetCities().size()) {
        ++city_idx;
    }
}

void MutableParameters::DecreaseCityIdx() {
    if (city_idx > 0) {
        --city_idx;
    }
}

size_t MutableParameters::GetCityIdx() const{
    return city_idx;
}

uint8_t MutableParameters::GetForecastDays() const{
    return forecast_days;
}

ImmutableParameters::ImmutableParameters(const std::vector<std::string>& cities, const size_t frequency, const std::string& api_key): cities(cities), frequency(frequency), api_key(api_key) {}

const std::vector<std::string>& ImmutableParameters::GetCities() const {
    return cities;
}

const std::string& ImmutableParameters::GetCityOnSomeIdx(size_t idx) const {
    return cities[idx];
}

const size_t ImmutableParameters::GetFrequency() const {
    return frequency;
}

const std::string& ImmutableParameters::GetAPIKey() const {
    return api_key;
}