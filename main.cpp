#include "ConfigInfo/ConfigInfo.h"
#include "API/API_Info.h"
#include "Interface/Interface.h"
#include "Parser/Parser.h"

const uint8_t kStartCityIdx = 0;

int main() {
    ConfigParameters user_config;
    if (!user_config.UnpackConfig()) {
        return 1;
    }

    ImmutableParameters user_immutable_parameters(user_config.cities, user_config.frequency, user_config.api_key);
    MutableParameters user_mutable_parameters(kStartCityIdx, user_config.forecast_days);

    APIInfo weather_data;
    weather_data.GetAllForecasts(user_immutable_parameters);

    Interface weather_display(user_immutable_parameters, user_mutable_parameters, weather_data);
    weather_display.OpenUI();
    return 0;
}
