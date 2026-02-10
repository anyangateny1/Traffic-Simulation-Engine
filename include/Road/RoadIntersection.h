#pragma once

#include "Geometry/Position.h"
#include "Identifiers/IDs.h"

struct RoadIntersection {
    RoadID a;
    RoadID b;
    Position pos;
};
