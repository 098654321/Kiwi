#include "./coord.hh"

#include "global/std/integer.hh"

namespace kiwi::hardware {

    Coord::Coord(std::i64 row, std::i64 col):
        row{row}, col{col} 
    {
    }

    bool Coord::operator == (const Coord& other) const {
        return this->col == other.col && this->row == other.row;
    }

}

namespace std {

    size_t hash<kiwi::hardware::Coord>::operator() (const kiwi::hardware::Coord& c) const noexcept {
        auto h = hash<i64>{};
        return h(c.col) ^ h(c.row);
    }

}