#pragma once

#include "../../global/std/integer.hh"
#include "../../global/std/collection.hh"
#include "../../global/std/string.hh"
#include <fstream>

namespace kiwi::circuit {

    using namespace std;

    class TopDie {
    public:
        TopDie(const String& name, HashMap<String, usize> pins_map);
        TopDie(const String& name);

        auto add_pin(const String& pin_name, const usize& index) -> void;

        auto checkInfo(std::ofstream&) -> void;

    private:
        std::String _name;
        std::HashMap<std::String, std::usize> _pins_map;
    };

}