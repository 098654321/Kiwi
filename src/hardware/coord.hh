#pragma once 

#include "../global/std/integer.hh"
#include <functional>

namespace kiwi::hardware {

    struct Coord {
        Coord(std::i64 row, std::i64 col);
        bool operator == (const Coord& other) const;

        std::i64 row;
        std::i64 col;
    };

}

namespace std {

    template<>
    struct hash<kiwi::hardware::Coord> {
        size_t operator() (const kiwi::hardware::Coord& c) const noexcept;
    };
    
}