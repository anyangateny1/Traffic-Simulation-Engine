#pragma once
#include "Geometry/Position.h"
#include <cmath>
#include <utility>
#include <vector>

class Road {
  public:
    Road(Position start_point,
         Position end_point,
         double length,
         std::vector<Position> curve_points)
        : start_point_(start_point), end_point_(end_point), length_(length),
          curve_points_(std::move(curve_points)) {}

    Road(const Road&) = default;
    Road(Road&&) noexcept = default;
    Road& operator=(const Road&) = default;
    Road& operator=(Road&&) noexcept = default;

    double Length() const noexcept { return length_; }

    const Position& StartPoint() const noexcept { return start_point_; }
    const Position& EndPoint() const noexcept { return end_point_; }

    const std::vector<Position>& CurvePoints() const noexcept { return curve_points_; }

    // Returns the (x, y) position at a given distance along the road
    const Position GetPositionAtDistance(double distance) const;

  private:
    Position start_point_;
    Position end_point_;
    double length_;
    std::vector<Position> curve_points_;
};
