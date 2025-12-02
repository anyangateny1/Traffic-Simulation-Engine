#pragma once
#include <cmath>
#include <utility>
#include <vector>

class Road {
  public:
    Road(std::pair<double, double> start_point,
         std::pair<double, double> end_point,
         double length,
         std::vector<std::pair<double, double>> curve_points)
        : start_point_(start_point),
          end_point_(end_point),
          length_(length),
          curve_points_(std::move(curve_points)) {}

    Road(const Road&) = default;
    Road(Road&&) noexcept = default;
    Road& operator=(const Road&) = default;
    Road& operator=(Road&&) noexcept = default;

    double Length() const noexcept { return length_; }
    
    const std::pair<double, double>& StartPoint() const noexcept { return start_point_; }
    const std::pair<double, double>& EndPoint() const noexcept { return end_point_; }
    
    const std::vector<std::pair<double, double>>& CurvePoints() const noexcept {
        return curve_points_;
    }

    // Returns the (x, y) position at a given distance along the road
    std::pair<double, double> GetPositionAtDistance(double distance) const;

  private:
    std::pair<double, double> start_point_;
    std::pair<double, double> end_point_;
    double length_;
    std::vector<std::pair<double, double>> curve_points_;
};
