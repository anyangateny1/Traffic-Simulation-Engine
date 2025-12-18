#include "Road/Road.h"
#include <algorithm>
#include <cmath>

const Position Road::GetPositionAtDistance(double distance) const {
    distance = std::clamp(distance, 0.0, length_);

    std::vector<Position> path;
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

        double dx = p2.x_coord - p1.x_coord;
        double dy = p2.y_coord - p1.y_coord;
        double segment_length = std::sqrt(dx * dx + dy * dy);

        if (accumulated_distance + segment_length >= distance) {
            double distance_into_segment = distance - accumulated_distance;
            double t = (segment_length > 0.0) ? (distance_into_segment / segment_length) : 0.0;

            double x = p1.x_coord + t * dx;
            double y = p1.y_coord + t * dy;
            return Position{x, y};
        }

        accumulated_distance += segment_length;
    }

    return end_point_;
}
