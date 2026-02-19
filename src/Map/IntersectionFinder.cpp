#include "Map/IntersectionFinder.h"

#include <algorithm>

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

std::vector<RoadIntersection>
IntersectionFinder::MergeHits(const std::vector<PairwiseHit>& hits) {
    constexpr double MERGE_EPS = 1e-6;

    std::vector<RoadIntersection> result;

    for (const auto& hit : hits) {
        RoadIntersection* existing = nullptr;
        for (auto& ri : result) {
            if ((ri.pos - hit.pos).magnitude_squared() < MERGE_EPS * MERGE_EPS) {
                existing = &ri;
                break;
            }
        }

        if (existing) {
            auto& ids = existing->intersecting_roads;
            if (std::find(ids.begin(), ids.end(), hit.road_a) == ids.end())
                ids.push_back(hit.road_a);
            if (std::find(ids.begin(), ids.end(), hit.road_b) == ids.end())
                ids.push_back(hit.road_b);
        } else {
            result.push_back(RoadIntersection{{hit.road_a, hit.road_b}, hit.pos});
        }
    }

    return result;
}

std::vector<RoadIntersection> IntersectionFinder::FindIntersections(const RoadGraph& graph) {
    const auto& roads_map = graph.GetRoads();

    std::vector<const Road*> roads;
    roads.reserve(roads_map.size());

    for (const auto& [id, road_ptr] : roads_map)
        roads.push_back(road_ptr.get());

    std::vector<PairwiseHit> hits;

    for (size_t i = 0; i < roads.size(); ++i) {
        for (size_t j = i + 1; j < roads.size(); ++j) {
            const Road* roadA = roads[i];
            const Road* roadB = roads[j];

            const auto& curveA = roadA->CurvePoints();
            const auto& curveB = roadB->CurvePoints();

            if (curveA.size() < 2 || curveB.size() < 2)
                continue;

            for (size_t a = 0; a + 1 < curveA.size(); ++a) {
                for (size_t b = 0; b + 1 < curveB.size(); ++b) {
                    auto intersection = SegmentIntersectionPoint(curveA[a], curveA[a + 1],
                                                                 curveB[b], curveB[b + 1]);

                    if (intersection) {
                        hits.push_back({roadA->Id(), roadB->Id(), *intersection});
                    }
                }
            }
        }
    }

    return MergeHits(hits);
}
