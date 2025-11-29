#pragma once
#include <utility>
#include <vector>

class Road {
  public:
    Road(int from_id, int to_id, double length, std::vector<std::pair<double, double>> curve_points)
        : from_id_(from_id), to_id_(to_id), length_(length),
          curve_points_(std::move(curve_points)) {}

    Road(const Road&) = default;
    Road(Road&&) noexcept = default;
    Road& operator=(const Road&) = default;
    Road& operator=(Road&&) noexcept = default;

    int FromId() const { return from_id_; }
    int ToId() const { return to_id_; }
    double Length() const { return length_; }
    const std::vector<std::pair<double, double>>& CurvePoints() const { return curve_points_; }

  private:
    int from_id_;
    int to_id_;
    double length_;
    std::vector<std::pair<double, double>> curve_points_;
};
