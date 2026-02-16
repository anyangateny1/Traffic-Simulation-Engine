file(GLOB_RECURSE LIB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(REMOVE_ITEM LIB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h"
)

# Create library
add_library(TrafficSimLib ${LIB_SOURCES} ${HEADERS})

target_include_directories(TrafficSimLib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(TrafficSimLib
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    nlohmann_json::nlohmann_json
)

set_target_properties(TrafficSimLib PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

add_executable(TrafficSimApp main.cpp)

target_link_libraries(TrafficSimApp
    PRIVATE
    TrafficSimLib
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)

target_include_directories(TrafficSimApp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
