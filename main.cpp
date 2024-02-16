#include "ConfigInfo/ConfigInfo.h"
#include "API/API_Info.h"
#include "Interface/Interface.h"

int main() {
    ConfigInfo user_info;
    if (!user_info.UnpackInfoFromUserJson()) {
        return 1;
    }

    API weather_forecasts(user_info.api_key);
    weather_forecasts.GetAllForecasts(user_info.cities);

    Interface weather_display(user_info, weather_forecasts);
    weather_display.OpenUI();
    return 0;
}
