#pragma once
#include <utility>
#include <vector>

class Road {
  public:
    Road(double length, std::vector<std::pair<double, double>> curve_points)
        : length_(length), curve_points_(std::move(curve_points)) {}

    Road(const Road&) = default;
    Road(Road&&) noexcept = default;
    Road& operator=(const Road&) = default;
    Road& operator=(Road&&) noexcept = default;

    double Length() const noexcept { return length_; }
    const std::vector<std::pair<double, double>>& CurvePoints() const noexcept { return curve_points_; }

  private:
    double length_;
    std::vector<std::pair<double, double>> curve_points_;
};
