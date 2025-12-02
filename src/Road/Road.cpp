#include "Road/Road.h"
#include <algorithm>
#include <cmath>

std::pair<double, double> Road::GetPositionAtDistance(double distance) const {
    distance = std::clamp(distance, 0.0, length_);

    std::vector<std::pair<double, double>> path;
    path.reserve(curve_points_.size() + 2);
    path.push_back(start_point_);
    for (const auto& point : curve_points_) {
        path.push_back(point);
    }
    path.push_back(end_point_);

    double accumulated_distance = 0.0;

    for (size_t i = 0; i < path.size() - 1; ++i) {
        const auto& p1 = path[i];
        const auto& p2 = path[i + 1];

        double dx = p2.first - p1.first;
        double dy = p2.second - p1.second;
        double segment_length = std::sqrt(dx * dx + dy * dy);

        if (accumulated_distance + segment_length >= distance) {
            double distance_into_segment = distance - accumulated_distance;
            double t = (segment_length > 0.0) ? (distance_into_segment / segment_length) : 0.0;

            double x = p1.first + t * dx;
            double y = p1.second + t * dy;
            return {x, y};
        }

        accumulated_distance += segment_length;
    }

    return end_point_;
}
