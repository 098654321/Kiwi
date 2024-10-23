#pragma once

#include "./topdie.hh"
#include "../net/net.hh"

#include "../../global/std/integer.hh"
#include "../../global/std/collection.hh"
#include "../../global/std/string.hh"
#include <fstream>

namespace kiwi::hardware {
    class TOB;
}

namespace kiwi::circuit {

    class TopDieInstance {

    public:
        TopDieInstance(
            const String& name, TopDie* topdie, hardware::TOB* tob
        );

        auto move_to_tob(hardware::TOB* tob) -> void;
        auto swap_tob_with(TopDieInstance* other) -> void;
        auto add_net(Net* pnet) -> void;
        auto checkInfo(std::ofstream&) -> void;
        
    public:
        auto topdie() const -> TopDie* { return this->_topdie; }
        auto tob() const -> hardware::TOB* { return this->_tob; }
        auto nets() const -> const std::Vector<Net*> { return this->_nets; }

    private:
        std::String _name;
        TopDie* _topdie;
        hardware::TOB* _tob;
        std::Vector<Net*> _nets; 
        
    };

}