#pragma once
#include "Geometry/Position.h"
#include "Road/Lane.h"
#include <cmath>
#include <span>
#include <utility>
#include <vector>

class Road {
  public:
    Road(Position start_point,
         Position end_point,
         double length,
         std::vector<Position> curve_points,
         std::span<const LaneConfig> lane_configs)
        : start_point_(start_point), end_point_(end_point), length_(length),
          curve_points_(std::move(curve_points)) {
        int index = 0;
        for (const auto& cfg : lane_configs) {
            lanes_.emplace_back(this, index++, cfg.dir, cfg.offset, cfg.width);
        }
    }

    Road(const Road&) = delete;
    Road(Road&&) = delete;
    Road& operator=(const Road&) = delete;
    Road& operator=(Road&&) = delete;

    double Length() const noexcept { return length_; }

    const Position& StartPoint() const noexcept { return start_point_; }
    const Position& EndPoint() const noexcept { return end_point_; }

    const std::vector<Position>& CurvePoints() const noexcept { return curve_points_; }

    // Returns the (x, y) position at a given distance along the road
    const Position GetPositionAtDistance(double distance) const;

  private:
    std::vector<Lane> lanes_;
    Position start_point_;
    Position end_point_;
    double length_;
    std::vector<Position> curve_points_;
};
