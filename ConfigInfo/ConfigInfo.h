#pragma once

#include <cstdint>
#include <vector>
#include <string>

class ImmutableParameters {
public:
    ImmutableParameters(const std::vector<std::string>& cities, const size_t frequency, const std::string& api_key);

    const std::vector<std::string>& GetCities() const;
    const std::string& GetCityOnSomeIdx(size_t idx) const;
    const size_t GetFrequency() const;
    const std::string& GetAPIKey() const;
private:
    const std::vector<std::string>& cities;
    const size_t frequency;
    const std::string& api_key;
};

class MutableParameters {
public:
    MutableParameters(size_t city_idx, uint8_t forecast_days);

    void IncreaseForecastDays();
    void DecreaseForecastDays();
    void IncreaseCityIdx(const ImmutableParameters& user_immutable_parameters);
    void DecreaseCityIdx();

    size_t GetCityIdx() const;
    uint8_t GetForecastDays() const;
private:
    size_t city_idx;
    uint8_t forecast_days;
};