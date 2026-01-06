#pragma once

#include "Identifiers/IDs.h"
#include <vector>

struct Path {
    std::vector<RoadID> roads;
    double total_distance{0.0};
};
