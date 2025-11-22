cmake_minimum_required(VERSION 3.24)
project(TrafficSim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------------
# Find Qt6
# -------------------------------
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# -------------------------------
# Add src folder
# -------------------------------
add_subdirectory(src)

# # -------------------------------
# # Add tests
# # -------------------------------
# enable_testing()
# add_subdirectory(tests)
