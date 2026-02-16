file(GLOB TEST_SOURCES "*.cpp")

add_executable(TrafficSimTests ${TEST_SOURCES})

target_link_libraries(TrafficSimTests
    PRIVATE
    TrafficSimLib
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(TrafficSimTests)
