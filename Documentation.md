# Documentation

This is a crossplatform (MacOS, Linux, Windows) console app that allows you to check the weather in cities.  
API-Ninja and OpenMeteo API are used here.  
In the config.json file you should set your parameters: set of cities, data update frequency, number of forecast days (since OpenMeteo API is used here, you should choose number not less than 0 and not more than 16) and API-Key which you can get by registering on API-Ninja website.  
You can move over your set of cities by pressing 'n' (next) or 'p' (previous) and also you can change number of forecast days by pressing '+' or '-'. To finish the program press Escape.  
This console app contains your chosen city, number of forecast days and the forecast itself. Each of days contains maximum and minimum temperatures (Celsium), maximum wind speed (km/h), rainfall (mm), which is called rain_sum in this app.  
Here is example:
![](https://i.ibb.co/BsJJLqW/photo-2024-02-16-03-08-18.jpg])
