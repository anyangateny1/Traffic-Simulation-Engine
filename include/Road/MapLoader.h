#pragma once
#include <filesystem>

class RoadGraph;

class MapLoader {
  public:
    MapLoader() = delete;

    // Static method: pass the graph to load into
    static bool LoadMapFromJson(RoadGraph& graph, const std::filesystem::path& filename);
};
