#pragma once
#include <cstdint>
#include <functional>

template <typename Tag> class StrongID {
  public:
    using value_type = std::uint32_t;

    constexpr StrongID() = default;
    explicit constexpr StrongID(value_type v) : value_(v) {}

    constexpr value_type value() const noexcept { return value_; }

    friend constexpr bool operator==(StrongID a, StrongID b) noexcept {
        return a.value_ == b.value_;
    }

    friend constexpr bool operator!=(StrongID a, StrongID b) noexcept { return !(a == b); }

  private:
    value_type value_{0};
};

namespace std {
template <typename Tag> struct hash<StrongID<Tag>> {
    size_t operator()(const StrongID<Tag>& id) const noexcept {
        return std::hash<typename StrongID<Tag>::value_type>{}(id.value());
    }
};
} // namespace std
