#include "Map/IntersectionFinder.h"

IntersectionFinder::IntersectionFinder() {}

std::optional<Position> IntersectionFinder::SegmentIntersectionPoint(const Position& A,
                                                                     const Position& B,
                                                                     const Position& C,
                                                                     const Position& D) noexcept {
    constexpr double EPS = 1e-9;

    const Position AB = B - A;
    const Position CD = D - C;

    const double denom = cross(AB, CD);

    if (std::abs(denom) < EPS)
        return std::nullopt;

    const Position AC = C - A;

    const double t = cross(AC, CD) / denom;
    const double u = cross(AC, AB) / denom;

    if (t <= EPS || t >= 1.0 - EPS || u <= EPS || u >= 1.0 - EPS)
        return std::nullopt;

    return A + AB * t;
}

std::vector<RoadIntersection> IntersectionFinder::FindIntersections(const RoadGraph& graph) {
    constexpr double EPS = 1e-9;

    std::vector<RoadIntersection> result;

    const auto& roads_map = graph.GetRoads();

    // Flatten map to vector
    std::vector<const Road*> roads;
    roads.reserve(roads_map.size());

    for (const auto& [id, road_ptr] : roads_map)
        roads.push_back(road_ptr.get());

    for (size_t i = 0; i < roads.size(); ++i) {
        for (size_t j = i + 1; j < roads.size(); ++j) {
            const Road* roadA = roads[i];
            const Road* roadB = roads[j];

            const auto& curveA = roadA->CurvePoints();
            const auto& curveB = roadB->CurvePoints();

            if (curveA.size() < 2 || curveB.size() < 2)
                continue;

            // Segment pair loop
            for (size_t a = 0; a + 1 < curveA.size(); ++a) {
                for (size_t b = 0; b + 1 < curveB.size(); ++b) {
                    auto intersection = SegmentIntersectionPoint(curveA[a], curveA[a + 1],
                                                                 curveB[b], curveB[b + 1]);

                    if (intersection) {
                        result.push_back(RoadIntersection{roadA->Id(), roadB->Id(), *intersection});
                    }
                }
            }
        }
    }

    return result;
}
