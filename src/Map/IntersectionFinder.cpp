#include "Map/IntersectionFinder.h"

IntersectionFinder::IntersectionFinder() {}

std::optional<Position> SegmentIntersectionPoint(const Position& A,
                                                 const Position& B,
                                                 const Position& C,
                                                 const Position& D) noexcept {
    Position AB = B - A;
    Position CD = D - C;

    double denom = cross(AB, CD);

    if (denom == 0.0)
        return std::nullopt;

    Position AC = C - A;
    double t = cross(AC, CD) / denom;
    double u = cross(AC, AB) / denom;

    if (t < 0.0 || t > 1.0 || u < 0.0 || u > 1.0)
        return std::nullopt;

    return A + AB * t;
}
