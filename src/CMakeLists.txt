# Recursively grab all .cpp files
file(GLOB_RECURSE SOURCES "*.cpp")

# Recursively grab all .h files from include directory
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h")

# Create library (include headers so AUTOMOC can process them)
add_library(TrafficSimLib ${SOURCES} ${HEADERS})

# Include headers from separate include folder
target_include_directories(TrafficSimLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Link Qt modules
target_link_libraries(TrafficSimLib PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGL Qt6::OpenGLWidgets)

# Enable Qt features
set_target_properties(TrafficSimLib PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# -------------------------------
# Add executable
# -------------------------------
add_executable(TrafficSimApp main.cpp)
target_link_libraries(TrafficSimApp PRIVATE TrafficSimLib Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGL Qt6::OpenGLWidgets)
target_include_directories(TrafficSimApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
