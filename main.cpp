#include "ConfigInfo/ConfigInfo.h"
#include "API/API_Info.h"
#include "Interface/Interface.h"
#include "Parser/Parser.h"

const uint8_t kStartCityIdx = 0;
const bool is_it_first_iteration = true;

int main() {
    ConfigParameters user_config;
    if (!user_config.UnpackConfig()) {
        return 1;
    }

    ImmutableParameters user_immutable_parameters(user_config.cities, user_config.frequency, user_config.api_key);
    MutableParameters user_mutable_parameters(kStartCityIdx, user_config.forecast_days);

    APIInfo weather_data;
    if (weather_data.GetAllForecasts(user_immutable_parameters, is_it_first_iteration) == 1) {
        return 1;
    }

    InterfacePresenter weather_image(user_immutable_parameters, user_mutable_parameters, weather_data);
    InterfaceController UI(weather_image);
    UI.OpenUI();
    return 0;
}
