#pragma once

#include "Geometry/Position.h"
#include "Road/RoadGraph.h"
#include "Road/RoadIntersection.h"
#include <optional>
#include <vector>

class IntersectionFinder {
  public:
    IntersectionFinder() = delete;

    static std::vector<RoadIntersection> FindIntersections(const RoadGraph& graph);

  private:
    static std::optional<Position> SegmentIntersectionPoint(const Position& A,
                                                            const Position& B,
                                                            const Position& C,
                                                            const Position& D) noexcept;

    struct PairwiseHit {
        RoadID road_a;
        RoadID road_b;
        Position pos;
    };

    static std::vector<RoadIntersection> MergeHits(const std::vector<PairwiseHit>& hits);
};
