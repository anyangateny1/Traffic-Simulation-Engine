#pragma once

#include "Identifiers/IDs.h"
#include "Pathfinding/Path.h"

class Pathfinder {
  public:
    virtual ~Pathfinder() = default;

    virtual Path FindPath(const NodeID& source, const NodeID& destination) const = 0;
};
