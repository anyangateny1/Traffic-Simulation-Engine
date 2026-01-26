#pragma once

#include "Identifiers/IDs.h"
#include "PathFinding/Path.h"

class PathFinder {
  public:
    virtual ~PathFinder() = default;

    virtual Path FindPath(const NodeID& source, const NodeID& destination) const = 0;
};
