
#pragma once

#include "../coord.hh"

#include <cstddef>
#include <functional>
#include "../../global/std/integer.hh"
#include "../../global/std/string.hh"

namespace kiwi::hardware {

    enum class TrackDirection {
        Horizontal,
        Vertical,
    };

    struct TrackCoord : public Coord {
        TrackCoord(std::i64 r, std::i64 c, TrackDirection d, std::usize i);
                
        bool operator == (const TrackCoord& other) const;
        auto to_string() const -> std::String;

        TrackDirection dir;
        std::usize index;
    };

}

namespace std {

    template <>
    struct hash<kiwi::hardware::TrackDirection> {
        std::size_t operator() (const kiwi::hardware::TrackDirection& dir) const noexcept;
    };

    template <>
    struct hash<kiwi::hardware::TrackCoord> {
        std::size_t operator() (const kiwi::hardware::TrackCoord& coord) const noexcept;
    };

}