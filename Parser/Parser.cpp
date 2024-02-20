#include "Parser.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
using json = nlohmann::json;

bool ConfigParameters::UnpackConfig() {
    std::ifstream file("..\\config.json");
    if (!file.is_open()) {
        std::cerr << "Either config.json isn't exist or you have remove config.json into another directory";
        return false;
    }
    json info = json::parse(file);
    cities = info["cities"];
    frequency = info["frequency"];
    forecast_days = info["forecast days"];
    api_key = info["API-Key"];
    if (cities.empty() || frequency == 0 || forecast_days == 0 || forecast_days > 16) {
        std::cerr << "Wrong input data, read the documentation to learn more about input requirements";
        return false;
    }

    if (!IsHaveInternetConnection()) {
        std::cerr << "Check for internet connection";
        return false;
    }

    if (!IsCorrectAPI()) {
        std::cerr << "Your API-Key is incorrect";
        return false;
    }
    return true;
}

bool ConfigParameters::IsCorrectAPI() {
    cpr::Response r = cpr::Get(cpr::Url("https://api.api-ninjas.com/v1/city"),
                               cpr::Header{{"X-Api-Key", api_key}});
    json check_the_correctness_of_api_key = json::parse(r.text);
    if (check_the_correctness_of_api_key.contains("error") && check_the_correctness_of_api_key["error"] == "Invalid API Key.") {
        return false;
    }
    return true;
}

bool ConfigParameters::IsHaveInternetConnection() {
    cpr::Response r = cpr::Get(cpr::Url("www.google.com"));
    if (r.text.empty()) {
        return false;
    }
    return true;
}