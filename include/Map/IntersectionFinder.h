#pragma once

#include "Geometry/Position.h"
#include <optional>
class IntersectionFinder {
  public:
    IntersectionFinder();
    IntersectionFinder(const IntersectionFinder&) = delete;
    IntersectionFinder(IntersectionFinder&&) = delete;
    IntersectionFinder& operator=(const IntersectionFinder&) = delete;
    IntersectionFinder& operator=(IntersectionFinder&&) = delete;

    std::optional<Position> SegmentIntersectionPoint(const Position& A,
                                                     const Position& B,
                                                     const Position& C,
                                                     const Position& D) noexcept;

  private:
};
