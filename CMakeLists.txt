cmake_minimum_required(VERSION 3.24)
project(TrafficSim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------------
# Find Qt6 and other packages
# -------------------------------
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)
# -------------------------------
# Add src folder
# -------------------------------
add_subdirectory(src)

# # -------------------------------
# # Add tests
# # -------------------------------

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4   # Use latest stable if needed
)

FetchContent_MakeAvailable(Catch2)

enable_testing()
add_subdirectory(tests)
