#pragma once

#include "Geometry/Position.h"
#include "Identifiers/IDs.h"
#include <vector>

struct RoadIntersection {
    std::vector<RoadID> intersecting_roads;
    Position pos;
};
