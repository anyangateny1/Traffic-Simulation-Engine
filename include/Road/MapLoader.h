#pragma once
#include <string_view>

class RoadGraph;

class MapLoader {
  public:
    MapLoader() = delete;

    // Static method: pass the graph to load into
    static bool LoadMapFromJson(RoadGraph& graph, std::string_view filename);
};
