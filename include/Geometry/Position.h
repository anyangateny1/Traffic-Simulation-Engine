#pragma once
#include <cmath>

struct Position {
    double x_coord{};
    double y_coord{};

    double magnitude() const noexcept { return std::sqrt(x_coord * x_coord + y_coord * y_coord); }

    double magnitude_squared() const noexcept { return x_coord * x_coord + y_coord * y_coord; }

    Position normalized() const noexcept {
        double m = magnitude();
        return m > 0.0 ? Position{x_coord / m, y_coord / m} : Position{};
    }

    bool operator==(const Position& other) const noexcept = default;
};

inline Position operator+(const Position& a, const Position& b) noexcept {
    return {a.x_coord + b.x_coord, a.y_coord + b.y_coord};
}

inline Position operator-(const Position& a, const Position& b) noexcept {
    return {a.x_coord - b.x_coord, a.y_coord - b.y_coord};
}

inline Position operator*(const Position& p, double scalar) noexcept {
    return {p.x_coord * scalar, p.y_coord * scalar};
}
