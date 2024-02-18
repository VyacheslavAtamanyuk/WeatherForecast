cmake_minimum_required(VERSION 3.20)
project(WeatherLab LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(cpr json ftxui)

add_executable(WeatherLab main.cpp
        ConfigInfo/ConfigInfo.cpp ConfigInfo/ConfigInfo.h
        API/API_Info.cpp API/API_Info.h
        Interface/Interface.cpp Interface/Interface.h
        Utils/Utils.cpp Utils/Utils.h
        Parser/Parser.cpp Parser/Parser.h)

target_link_libraries(WeatherLab PUBLIC
        ftxui::screen
        ftxui::dom
        ftxui::component # Not needed for this example.
)
target_link_libraries(WeatherLab PRIVATE cpr::cpr)
target_link_libraries(WeatherLab PRIVATE nlohmann_json::nlohmann_json)