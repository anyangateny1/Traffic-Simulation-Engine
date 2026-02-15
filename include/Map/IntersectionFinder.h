#pragma once

#include "Geometry/Position.h"
#include "Road/RoadGraph.h"
#include "Road/RoadIntersection.h"
#include <optional>
#include <vector>
class IntersectionFinder {
  public:
    IntersectionFinder();
    IntersectionFinder(const IntersectionFinder&) = delete;
    IntersectionFinder(IntersectionFinder&&) = delete;
    IntersectionFinder& operator=(const IntersectionFinder&) = delete;
    IntersectionFinder& operator=(IntersectionFinder&&) = delete;

    std::vector<RoadIntersection> FindIntersections(const RoadGraph& graph);

  private:
    std::optional<Position> SegmentIntersectionPoint(const Position& A,
                                                     const Position& B,
                                                     const Position& C,
                                                     const Position& D) noexcept;
};
