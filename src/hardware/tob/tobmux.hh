#pragma once

#include "./tobregister.hh"
#include "global/std/collection.hh"
#include "global/std/integer.hh"
#include "global/std/utility.hh"

namespace kiwi::hardware {

    class TOBMuxConnector {
    public:
        TOBMuxConnector(std::usize input_index, std::usize output_index, TOBMuxRegister& reg);

        auto connect() -> void;
        auto disconnect() -> void;

        auto input_index() const -> std::usize { return this->_input_index; }
        auto output_index() const -> std::usize { return this->_output_index; }

    private:
        std::usize _input_index;
        std::usize _output_index;
        TOBMuxRegister& _register;
    };

    class TOBMux {
    public:
        TOBMux(std::usize mux_size);

    public:
        auto available_connectors(std::usize input_index) -> std::Vector<TOBMuxConnector>;
        auto available_output_indexes() -> std::Vector<std::usize>;

    public:
        auto randomly_map_remain_indexes() -> void;
    
    public:
        auto index_map(std::usize input_index) const -> std::Option<std::usize>;
        auto registerr(std::usize input_index) -> TOBMuxRegister&;

        // updated by tang, 2024-09-28
        auto whole_reg() -> std::Vector<int>;
        //
    
    private:
        std::usize _mux_size;
        std::Vector<TOBMuxRegister> _registers;
    };

}