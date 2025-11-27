#pragma once
#include "Road/RoadGraph.h"
#include <string_view>

class MapLoader {
  public:
    explicit MapLoader(RoadGraph& g) noexcept : graph(g) {}

    bool loadMapFromJson(std::string_view filename);

    MapLoader(const MapLoader&) = delete;
    MapLoader(MapLoader&&) = delete;
    MapLoader& operator=(const MapLoader&) = delete;
    MapLoader& operator=(MapLoader&&) = delete;

  private:
    RoadGraph& graph;
};
