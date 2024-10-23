#pragma once

#include "./topdie/topdie.hh"
#include "./topdie/topdieinst.hh"
#include "../global/std/memory.hh"

#include "../global/std/integer.hh"
#include "../global/std/collection.hh"
#include "../global/std/string.hh"

namespace kiwi::circuit {

    class BaseDie {
    public:
        inline auto get_topdies() -> std::HashMap<std::String, TopDie>& { return this->_topdies;} 
        inline auto get_topdieinsts() -> std::HashMap<std::String, TopDieInstance>& {return this->_topdie_insts;}
        inline auto get_nets() -> std::Vector<std::Box<Net>>& {return this->_nets;}

        inline auto set_net(std::Vector<std::Box<Net>>& other) -> void;

    private:
        std::HashMap<std::String, TopDie> _topdies;
        std::HashMap<std::String, TopDieInstance> _topdie_insts;
        std::Vector<std::Box<Net>> _nets;   
    };

}